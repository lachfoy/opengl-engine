#include "Shader.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
	std::string vertexCode;
	std::string fragmentCode;
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;

	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	// Read in the shader source files
	try
	{
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);

		std::stringstream vShaderStream, fShaderStream;
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();

		vShaderFile.close();
		fShaderFile.close();

		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ" << std::endl;
	}

	// Create shaders
	GLuint vertex = glCreateShader(GL_VERTEX_SHADER);
	GLuint 	fragment = glCreateShader(GL_FRAGMENT_SHADER);

	GLint success = GL_FALSE;
	int infoLogLength;

	// Compile vertex shader
	const char* vShaderCode = vertexCode.c_str();
	glShaderSource(vertex, 1, &vShaderCode, NULL);
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
	const char* fShaderCode = fragmentCode.c_str();
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);

	// Check fragment shader
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	glGetShaderiv(fragment, GL_INFO_LOG_LENGTH, &infoLogLength);
	if (infoLogLength > 0)
	{
		std::vector<char> fragmentShaderErrorMessage(infoLogLength + 1);
		glGetShaderInfoLog(vertex, infoLogLength, NULL, &fragmentShaderErrorMessage[0]);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << &fragmentShaderErrorMessage[0] << std::endl;
	}

	// Link compiled vertex and fragment shaders into shader program
	GLuint programID = glCreateProgram();
	glAttachShader(programID, vertex);
	glAttachShader(programID, fragment);
	glLinkProgram(programID);

	// Check the program
	glGetProgramiv(programID, GL_LINK_STATUS, &success);
	glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &infoLogLength);
	if (!success)
	{
		std::vector<char> programErrorMessage(infoLogLength + 1);
		glGetProgramInfoLog(programID, infoLogLength, NULL, &programErrorMessage[0]);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << &programErrorMessage[0] << std::endl;
	}

	// Shader objects are now no longer needed
	glDetachShader(programID, vertex);
	glDetachShader(programID, fragment);

	glDeleteShader(vertex);
	glDeleteShader(fragment);

	this->ID = programID;
}

Shader::Shader(const char* vertexPath, const char* fragmentPath, const char* geometryPath)
{
	std::string vertexCode;
	std::string fragmentCode;
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;
	std::string geometryCode;
	std::ifstream gShaderFile;

	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	gShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	// Read in the shader source files
	try
	{
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);
		gShaderFile.open(geometryPath);

		std::stringstream vShaderStream, fShaderStream, gShaderStream;
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();
		gShaderStream << gShaderFile.rdbuf();

		vShaderFile.close();
		fShaderFile.close();
		gShaderFile.close();

		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
		geometryCode = gShaderStream.str();
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ" << std::endl;
	}

	// Create shaders
	GLuint vertex = glCreateShader(GL_VERTEX_SHADER);
	GLuint 	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	GLuint 	geometry = glCreateShader(GL_GEOMETRY_SHADER);

	GLint success = GL_FALSE;
	int infoLogLength;

	// Compile vertex shader
	const char* vShaderCode = vertexCode.c_str();
	glShaderSource(vertex, 1, &vShaderCode, NULL);
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
	const char* fShaderCode = fragmentCode.c_str();
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

	// Compile geometry shader
	const char* gShaderCode = geometryCode.c_str();
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

	// Link compiled vertex and fragment shaders into shader program
	GLuint programID = glCreateProgram();
	glAttachShader(programID, vertex);
	glAttachShader(programID, fragment);
	glAttachShader(programID, geometry);
	glLinkProgram(programID);

	// Check the program
	glGetProgramiv(programID, GL_LINK_STATUS, &success);
	glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &infoLogLength);
	if (!success)
	{
		std::vector<char> programErrorMessage(infoLogLength + 1);
		glGetProgramInfoLog(programID, infoLogLength, NULL, &programErrorMessage[0]);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << &programErrorMessage[0] << std::endl;
	}

	// Shader objects are now no longer needed
	glDetachShader(programID, vertex);
	glDetachShader(programID, fragment);
	glDetachShader(programID, geometry);

	glDeleteShader(vertex);
	glDeleteShader(fragment);
	glDeleteShader(geometry);

	this->ID = programID;
}

void Shader::use()
{
	glUseProgram(ID);
}

void Shader::setBool(const std::string& name, bool value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void Shader::setInt(const std::string& name, int value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setFloat(const std::string& name, float value) const
{
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setVec3(const std::string& name, const glm::vec3& value) const
{
	glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}

void Shader::setMat4(const std::string& name, const glm::mat4& mat) const
{
	glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}
