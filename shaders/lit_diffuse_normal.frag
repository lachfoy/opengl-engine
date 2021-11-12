#version 330 core
out vec4 FragColor;

struct Material {
	sampler2D diffuse;
	sampler2D normal;
	vec3 specular;
	float shininess;
};

struct DirLight {
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
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
uniform vec3 lightPos; // TEMPORARY, TESTING SHADOW MAP
uniform sampler2D shadowMap;
uniform Material material;
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform DirLight dirLight;

float CalcShadows();
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
	vec3 lightDir = normalize(-light.direction);
	// diffuse
	float diff = max(dot(normal, lightDir), 0.0);

	// spec
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	
	// combine results
	vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
	vec3 specular = light.specular * (spec * material.specular);
	
	// Calculate shaddows
	float shadow = CalcShadows();

	return (ambient + (1.0f - shadow) * diffuse + specular * (1.0f - shadow));
}

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

	return (ambient + diffuse + specular);
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

float CalcShadows()
{
	float shadow = 0.0f;
	// perspective divide 
	vec3 projCoords = FragPosLightSpace.xyz / FragPosLightSpace.w;
	if (projCoords.z <= 1.0f)
	{
		// transform to 0,1 range
		projCoords = projCoords * 0.5f + 0.5f;

		// get depth of current fragment from light perspective
		float currentDepth = projCoords.z;

		// Calculate bias
		vec3 normal = normalize(Normal);
		vec3 lightDir = normalize(lightPos - FragPos);
		float bias = max(0.025f * (1.0f - dot(normal, lightDir)), 0.0005f);

		// filtering
		int sampleRadius = 2;
		vec2 pixelSize = 1.0 / textureSize(shadowMap, 0);
		for (int y = -sampleRadius; y <= sampleRadius; y++)
			for (int x = -sampleRadius; x <= sampleRadius; x++)
			{
				float closestDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * pixelSize).r;
					if (currentDepth > closestDepth + bias)
						shadow += 1.0f;
			}

		// get average shadow
		shadow /= pow((sampleRadius * 2 + 1), 2);
	}

	return shadow;
}

void main()
{
	// properties
	//vec3 norm = normalize(Normal);
	vec3 normal = CalcBumpedNormal();

	vec3 viewDir = normalize(viewPos - FragPos);

	// Directional Lighting
	vec3 result = CalcDirLight(dirLight, normal, viewDir);

	// Point lights
	//for (int i = 0; i < NR_POINT_LIGHTS; i++)
	//	result += CalcPointLight(pointLights[i], normal, FragPos, viewDir);

	FragColor = vec4(result, 1.0);
}