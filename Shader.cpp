#include "Shader.h"

#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <vector>

Shader& Shader::use()
{
	glUseProgram(this->ID);
	return *this;
}

void Shader::Use()
{
	glUseProgram(ID);
}

void Shader::compile(const char* vertexSrc, const char* fragmentSrc, const char* geometrySrc)
{
	// Create shaders
	unsigned int vertex = glCreateShader(GL_VERTEX_SHADER);
	unsigned int fragment = glCreateShader(GL_FRAGMENT_SHADER);
	unsigned int geometry = glCreateShader(GL_GEOMETRY_SHADER);
	ID = glCreateProgram();

	GLint success = GL_FALSE;
	int infoLogLength;

	// Compile vertex shader
	glShaderSource(vertex, 1, &vertexSrc, NULL);
	glCompileShader(vertex);

	// Check vertex shader
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	glGetShaderiv(vertex, GL_INFO_LOG_LENGTH, &infoLogLength);
	if (infoLogLength > 0)
	{
		std::vector<char> vertexShaderErrorMessage(infoLogLength + 1);
		glGetShaderInfoLog(vertex, infoLogLength, NULL, &vertexShaderErrorMessage[0]);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << &vertexShaderErrorMessage[0] << std::endl;
	}

	// Compile fragment shader
	const char* fShaderCode = fragmentSrc;
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);

	// Check fragment shader
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	glGetShaderiv(fragment, GL_INFO_LOG_LENGTH, &infoLogLength);
	if (infoLogLength > 0)
	{
		std::vector<char> fragmentShaderErrorMessage(infoLogLength + 1);
		glGetShaderInfoLog(fragment, infoLogLength, NULL, &fragmentShaderErrorMessage[0]);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << &fragmentShaderErrorMessage[0] << std::endl;
	}

	// If geometry shader is not null, compile geometry shader
	if (geometrySrc != nullptr)
	{
		const char* gShaderCode = geometrySrc;
		glShaderSource(geometry, 1, &gShaderCode, NULL);
		glCompileShader(geometry);

		// Check geometry shader
		glGetShaderiv(geometry, GL_COMPILE_STATUS, &success);
		glGetShaderiv(geometry, GL_INFO_LOG_LENGTH, &infoLogLength);
		if (infoLogLength > 0)
		{
			std::vector<char> geometryShaderErrorMessage(infoLogLength + 1);
			glGetShaderInfoLog(geometry, infoLogLength, NULL, &geometryShaderErrorMessage[0]);
			std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << &geometryShaderErrorMessage[0] << std::endl;
		}
	}

	// Link compiled vertex and fragment shaders into shader program
	glAttachShader(ID, vertex);
	glAttachShader(ID, fragment);
	if (geometrySrc != nullptr) glAttachShader(ID, geometry);
	glLinkProgram(ID);

	// Check the program
	glGetProgramiv(ID, GL_LINK_STATUS, &success);
	glGetProgramiv(ID, GL_INFO_LOG_LENGTH, &infoLogLength);
	if (!success)
	{
		std::vector<char> programErrorMessage(infoLogLength + 1);
		glGetProgramInfoLog(ID, infoLogLength, NULL, &programErrorMessage[0]);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << &programErrorMessage[0] << std::endl;
	}

	// Shader objects are now no longer needed
	//glDetachShader(ID, vertex);
	//glDetachShader(ID, fragment);
	//if (geometrySrc != nullptr) glDetachShader(ID, geometry);

	glDeleteShader(vertex);
	glDeleteShader(fragment);
	if (geometrySrc != nullptr) glDeleteShader(geometry);
}

void Shader::setInt(const std::string& name, int value, bool useShader)
{
	if (useShader) this->use();
	glUniform1i(glGetUniformLocation(this->ID, name.c_str()), value);
}

void Shader::setFloat(const std::string& name, float value, bool useShader)
{
	if (useShader) this->use();
	glUniform1f(glGetUniformLocation(this->ID, name.c_str()), value);
}

void Shader::setVec3(const std::string& name, const glm::vec3& value, bool useShader)
{
	if (useShader) this->use();
	glUniform3f(glGetUniformLocation(this->ID, name.c_str()), value.x, value.y, value.z);
}

void Shader::setMat4(const std::string& name, const glm::mat4& mat, bool useShader)
{
	if (useShader) this->use();
	glUniformMatrix4fv(glGetUniformLocation(this->ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
}
