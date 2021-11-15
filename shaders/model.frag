#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;
in vec3 Tangent;

uniform sampler2D diffuse;

void main()
{
	FragColor = texture(diffuse, TexCoords);
}