#version 330 core
out vec4 FragColor;

struct Material {
	sampler2D diffuse;
	sampler2D normal;
	vec3 specular;
	float shininess;
};

struct PointLight {
	vec3 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;
};
#define NR_POINT_LIGHTS 1

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;
in vec3 Tangent;
in vec4 FragPosLightSpace;

uniform vec3 viewPos;
uniform samplerCube shadowCubeMap;
uniform float far_plane;
uniform Material material;
uniform PointLight pointLights[NR_POINT_LIGHTS];

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	vec3 lightDir = normalize(light.position - fragPos);
	// diffuse
	float diff = max(dot(normal, lightDir), 0.0);
	
	// specular
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir,reflectDir), 0.0), material.shininess);
	
	// attentuation
	float distance = length(light.position - FragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

	// Combine results
	vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
	vec3 specular = light.specular * (spec * material.specular);

	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

	// shadow
	float shadow = 0.0f;
	vec3 fragToLight = fragPos - light.position;
	float currentDepth = length(fragToLight);

	// Calculate bias
	float bias = max(0.5f * (1.0f - dot(normalize(Normal), lightDir)), 0.05f);

	// filtering
	int sampleRadius = 2;
	float offset = 0.02f;
	for (int z = -sampleRadius; z <= sampleRadius; z++)
		for (int y = -sampleRadius; y <= sampleRadius; y++)
			for (int x = -sampleRadius; x <= sampleRadius; x++)
			{
				float closestDepth = texture(shadowCubeMap, fragToLight + vec3(x, y, z) * offset).r;
				closestDepth *= far_plane;	
				if (currentDepth > closestDepth + bias)
					shadow += 1.0f;
			}
	
	// get average shadow
	shadow /= pow((sampleRadius * 2 + 1), 3);

	return (ambient + diffuse * (1.0f - shadow) + specular * (1.0f - shadow));
	//return (ambient + diffuse + specular);
}

vec3 CalcBumpedNormal()
{
    vec3 normal = normalize(Normal);
    vec3 tangent = normalize(Tangent);
    tangent = normalize(tangent - dot(tangent, normal) * normal);
    vec3 bitangent = cross(tangent, normal);
    vec3 BumpMapNormal = texture(material.normal, TexCoords).xyz;
    BumpMapNormal = 2.0 * BumpMapNormal - vec3(1.0, 1.0, 1.0);
    vec3 NewNormal;
    mat3 TBN = mat3(tangent, bitangent, normal);
    NewNormal = TBN * BumpMapNormal;
    NewNormal = normalize(NewNormal);
    return NewNormal;
}

void main()
{
	// properties
	//vec3 norm = normalize(Normal);
	vec3 normal = CalcBumpedNormal();

	vec3 viewDir = normalize(viewPos - FragPos);

	// Point lights
	vec3 result = CalcPointLight(pointLights[0], normal, FragPos, viewDir);
	//for (int i = 0; i < NR_POINT_LIGHTS; i++)
	//	result += CalcPointLight(pointLights[i], normal, FragPos, viewDir);

	FragColor = vec4(result, 1.0);
}