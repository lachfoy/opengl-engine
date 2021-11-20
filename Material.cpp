#include "Material.h"

Material::Material(Shader shader, Texture* texture)
	: shader{ shader }, texture{ texture }
{
}

Material::Material(std::shared_ptr<Shader> shader, std::shared_ptr<Texture> texture)
	: mShader { shader }, mTexture { texture }
{
}

Material::~Material() {}

void Material::setShader(Shader* shader)
{
	//this->shader = shader;
}

void Material::setTexture(Texture* texture)
{
	this->texture = texture;
}

void Material::setShader(std::shared_ptr<Shader> shader)
{
	mShader = shader;
}

void Material::setTexture(std::shared_ptr<Texture> texture)
{
	mTexture = texture;
}

void Material::updateUniforms()
{
	for (const auto& it : uniformInts)
	{
		mShader->use().setInt(it.first, it.second);
	}
	for (const auto& it : uniformFloats)
	{
		mShader->use().setFloat(it.first, it.second);
	}
	for (const auto& it : uniformVec3s)
	{
		mShader->use().setVec3(it.first, it.second);
	}
	for (const auto& it : uniformMat4s)
	{
		mShader->use().setMat4(it.first, it.second);
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
