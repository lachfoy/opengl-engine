#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform samplerCube screenTexture;

void main()
{
	float phi = TexCoords.x * 3.1415*2;
	float theta = (-TexCoords.y + 0.5) * 3.1415;

	vec3 dir = vec3(cos(phi) * cos(theta), sin(theta), sin(phi) * cos(theta));

	FragColor = vec4(vec3(texture(screenTexture, dir).r), 1.0);
}