#include "Mesh.h"

Mesh::Mesh()
{

}

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices)
{
	this->vertices = vertices;
	this->indices = indices;
}

void Mesh::setUpMesh()
{
	if (this->VAO == 0)
	{
		// Generate the buffer objects
		glGenVertexArrays(1, &this->VAO);
		glGenBuffers(1, &this->VBO);
		glGenBuffers(1, &this->EBO);
	}

	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s)
	glBindVertexArray(this->VAO);
	// copy vertices array into buffer
	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
	glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(Vertex), &this->vertices[0], GL_STATIC_DRAW);

	if (this->indices.size() > 0)
	{
		// copy indices array into buffer
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(unsigned int), &this->indices[0], GL_STATIC_DRAW);
	}

	// set vertex attribute pointers so that OpenGL understands the format of our vertex data
	// Position attribute
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);

	// Normal attribute
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, Normal));

	// Texture coord attribute
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, TexCoords));

	glBindVertexArray(0);
}