#version 330 core
out vec4 FragColor;

//in vec3 FragPos;
//in vec3 Normal;
in vec2 TexCoords;

uniform sampler2D diffuse;

void main()
{
	FragColor = vec4(vec3(1.0, 1.0, 1.0), 1.0);//texture(diffuse, TexCoords);
}