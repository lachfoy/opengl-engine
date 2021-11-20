#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "Shader.h"
#include "Texture.h"

#include <map>
#include <string>
#include <memory>

// a shader with a stored list of properties/uniforms and some render settings (like depth test, blend, cull options)
class Material
{
public:
	Material() {}
	Material(Shader shader, Texture* texture = nullptr);
	Material(std::shared_ptr<Shader> shader, std::shared_ptr<Texture> texture = nullptr);
	~Material();
	
	inline Shader getShader() const { return this->shader; }
	inline std::shared_ptr<Shader> getShaderPointer() const { return mShader; }
	inline Texture* getTexture() const { return this->texture; }

	void setShader(Shader* shader);
	void setTexture(Texture* texture);
	void setShader(std::shared_ptr<Shader> shader);
	void setTexture(std::shared_ptr<Texture> texture);

	void updateUniforms();

	void setInt(std::string name, int value);
	void setFloat(std::string name, float value);
	void setVec3(std::string name, glm::vec3 value);
	void setMat4(std::string name, glm::mat4 value);

private:
	Shader shader;
	std::shared_ptr<Shader> mShader;
	Texture* texture;
	std::shared_ptr<Texture> mTexture;
	glm::vec3 color;

	std::map<std::string, int> uniformInts;
	std::map<std::string, float> uniformFloats;
	std::map<std::string, glm::vec3> uniformVec3s;
	std::map<std::string, glm::mat4> uniformMat4s;
};