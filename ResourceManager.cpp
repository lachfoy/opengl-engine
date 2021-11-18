#include "ResourceManager.h"

#include "stb_image.h"
#include "tiny_obj_loader.h"

#include <iostream>
#include <sstream>
#include <fstream>

std::map<std::string, Texture> ResourceManager::textures;
std::map<std::string, Shader> ResourceManager::shaders;
std::map<std::string, Mesh> ResourceManager::meshes;
std::map<std::string, Material> ResourceManager::materials;

Shader ResourceManager::loadShader(const char* vShaderFile, const char* fShaderFile, const char* gShaderFile, std::string name)
{
	shaders[name] = loadShaderFromFile(vShaderFile, fShaderFile, gShaderFile);
	return shaders[name];
}

Shader ResourceManager::getShader(std::string name)
{
	return shaders[name];
}

Texture ResourceManager::loadTexture(const char* file, bool alpha, std::string name)
{
	textures[name] = loadTextureFromFile(file, alpha);
	return textures[name];
}

Texture ResourceManager::getTexture(std::string name)
{
	return textures[name];
}

Mesh ResourceManager::loadMesh(const char* file, std::string name)
{
	meshes[name] = loadMeshFromFile(file);
	return meshes[name];
}

Mesh ResourceManager::getMesh(std::string name)
{
	return meshes[name];
}

Material ResourceManager::getMaterial(std::string name)
{
	return materials[name];
}

void ResourceManager::clear()
{
	for (auto shader : shaders) glDeleteProgram(shader.second.ID);
	for (auto texture : textures) glDeleteTextures(1, &texture.second.ID);
}

Shader ResourceManager::loadShaderFromFile(const char* vShaderFile, const char* fShaderFile, const char* gShaderFile)
{
	std::string vertexCode;
	std::string fragmentCode;
	std::string geometryCode;
	
	std::ifstream vertexShaderFile;
	std::ifstream fragmentShaderFile;
	std::ifstream geometryShaderFile;

	vertexShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fragmentShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	geometryShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	// Read in the shader source files
	try
	{
		vertexShaderFile.open(vShaderFile);
		fragmentShaderFile.open(fShaderFile);

		std::stringstream vShaderStream, fShaderStream;
		vShaderStream << vertexShaderFile.rdbuf();
		fShaderStream << fragmentShaderFile.rdbuf();

		vertexShaderFile.close();
		fragmentShaderFile.close();

		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
		
		if (gShaderFile != nullptr)
		{
			geometryShaderFile.open(gShaderFile);

			std::stringstream gShaderStream;
			gShaderStream << geometryShaderFile.rdbuf();

			geometryShaderFile.close();

			geometryCode = gShaderStream.str();
		}
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ" << std::endl;
	}

	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();
	const char* gShaderCode = geometryCode.c_str();

	Shader shader;
	shader.compile(vShaderCode, fShaderCode, gShaderFile != nullptr ? gShaderCode : nullptr);
	return shader;
}

Texture ResourceManager::loadTextureFromFile(const char* file, bool alpha)
{
	Texture texture;
	if (alpha)
	{
		texture.Internal_Format = GL_RGBA;
		texture.Image_Format = GL_RGBA;
	}

	int width, height, nChannels;
	unsigned char* data = stbi_load(file, &width, &height, &nChannels, 0);

	texture.generate(width, height, data);

	stbi_image_free(data);

	return texture;
}

Mesh ResourceManager::loadMeshFromFile(const char* file)
{
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string warn, err;

	if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, file))
	{
		throw std::runtime_error(warn + err);
	}

	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	for (const auto& shape : shapes)
	{
		for (const auto& index : shape.mesh.indices)
		{
			Vertex vertex{};

			vertex.Position = {
				attrib.vertices[3 * index.vertex_index + 0],
				attrib.vertices[3 * index.vertex_index + 1],
				attrib.vertices[3 * index.vertex_index + 2]
			};

			if (index.normal_index >= 0)
			{
				vertex.Normal = {
					attrib.normals[3 * index.normal_index + 0],
					attrib.normals[3 * index.normal_index + 1],
					attrib.normals[3 * index.normal_index + 2],

				};
			}

			if (index.texcoord_index >= 0)
			{
				vertex.TexCoords = {
					attrib.texcoords[2 * index.texcoord_index + 0],
					attrib.texcoords[2 * index.texcoord_index + 1]
				};
			}

			vertices.push_back(vertex);
			indices.push_back(indices.size());
		}
	}

	Mesh mesh(vertices, indices);
	mesh.setUpMesh();

	return mesh;
}
