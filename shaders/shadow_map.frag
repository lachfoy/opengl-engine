#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D shadowMap;

void main()
{
	float depth = texture(shadowMap, TexCoords).x;
	depth = 1.0 - (1.0 - depth) * 25.0;
	FragColor = vec4(depth);
}