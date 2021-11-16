#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <iostream>
#include <vector>

#include "Texture.h"

struct Vertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
};

class Model
{
public:
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	Texture texture; // TODO: should be replaced with a material class?
	unsigned int VAO;
	
	Model() { VAO = 0; VBO = 0; EBO = 0; }
	~Model();

	void setTexture(Texture& texture);
	void generate(std::vector<Vertex> vertices, std::vector<unsigned int> indices);

private:
	unsigned int VBO, EBO;

	void setUpModel();
};
