#version 330 core
out vec4 FragColor;

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
} fs_in;

struct Light {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform sampler2D diffuse;

uniform Light light;

uniform vec3 objectColor;
uniform vec3 viewPos;

void main()
{
    //vec3 objectColor = vec3(1.0f, 1.0f, 1.0f);

    vec3 ambient = light.ambient * objectColor;
    // diffuse lighting
    vec3 norm = normalize(fs_in.Normal);
    vec3 lightDir = normalize(-light.direction);
    float diff = max(dot(norm, lightDir), 0.0f);
    vec3 diffuseResult = light.diffuse * diff * objectColor;
    vec3 result = ambient + diffuseResult;
	FragColor = vec4(result, 1.0f); // texture(diffuse, TexCoords);
}