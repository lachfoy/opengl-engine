#include "Material.h"

Material::Material(Shader shader, Texture* texture)
{
	this->shader = shader;
	this->texture = texture;
}

Material::~Material()
{
	//delete this->shader;
}

void Material::setShader(Shader* shader)
{
	//this->shader = shader;
}

void Material::setTexture(Texture* texture)
{
	this->texture = texture;
}

void Material::updateUniforms()
{
	for (const auto& it : uniformInts)
	{
		shader.setInt(it.first, it.second);
	}
	for (const auto& it : uniformFloats)
	{
		shader.setFloat(it.first, it.second);
	}
	for (const auto& it : uniformVec3s)
	{
		shader.setVec3(it.first, it.second);
	}
	for (const auto& it : uniformMat4s)
	{
		shader.setMat4(it.first, it.second);
	}
}

void Material::setInt(std::string name, int value)
{
	uniformInts[name] = value;
}

void Material::setFloat(std::string name, float value)
{
	uniformFloats[name] = value;
}

void Material::setVec3(std::string name, glm::vec3 value)
{
	uniformVec3s[name] = value;
}

void Material::setMat4(std::string name, glm::mat4 value)
{
	uniformMat4s[name] = value;
}
