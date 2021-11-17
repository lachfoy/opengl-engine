#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <vector>

enum Topology
{
	TRIANGLES,
	TRIANGLE_STRIP,
};

struct Vertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
};

class Mesh
{
public:
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	unsigned int VAO = 0;
	Topology topology = TRIANGLES;
	
	Mesh();
	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices);

	void setUpMesh();

private:
	unsigned int VBO, EBO;

	// CalculateNormals(); // implementing this would be useful
	// CalculateTangents();
};
