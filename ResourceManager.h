#pragma once

#include <glad/glad.h>

#include <map>
#include <string>

#include "Texture.h"
#include "Shader.h"
#include "Mesh.h"
#include "Cube.h"
#include "Material.h"

class ResourceManager
{
public:
	static std::map<std::string, Shader> shaders;
	static std::map<std::string, Texture> textures;
	static std::map<std::string, Mesh> meshes;
	static std::map<std::string, Material> materials;

	static Shader loadShader(const char* vShaderFile, const char * fShaderFile, const char *gShaderFile, std::string name);
	static Shader getShader(std::string name);
	static Texture loadTexture(const char* file, bool alpha, std::string name);
	static Texture getTexture(std::string name);
	static Mesh loadMesh(const char* file, std::string name);
	static Mesh getMesh(std::string name);
	static void createDefaultMaterials();
	static Material getMaterial(std::string name);
	static void clear();

private:
	ResourceManager() { }

	static Shader loadShaderFromFile(const char* vShaderFile, const char* fShaderFile, const char* gShaderFile = nullptr);
	static Texture loadTextureFromFile(const char* file, bool alpha);
	static Mesh loadMeshFromFile(const char* file);
};