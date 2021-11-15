#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>

class Shader
{
public:
	GLuint ID;

	Shader() { ID = 0; }
	Shader& use();

	void compile(const char* vertexSrc, const char* fragmentSrc, const char* geometrySrc = nullptr);

	void setInt(const std::string& name, int value, bool useShader = false);
	void setFloat(const std::string& name, float value, bool useShader = false);
	void setVec3(const std::string& name, const glm::vec3& value, bool useShader = false);
	void setMat4(const std::string& name, const glm::mat4 &mat, bool useShader = false);
};