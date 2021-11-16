#version 330 core
out vec4 FragColor;

//in vec3 FragPos;
//in vec3 Normal;
in vec2 TexCoords;

uniform sampler2D diffuse;

void main()
{
	FragColor =vec4(1.0f, 1.0f, 1.0f, 1.0f);// texture(diffuse, TexCoords);
}