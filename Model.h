#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "tiny_obj_loader.h"

#include <iostream>
#include <vector>

#include "Shader.h"
#include "Texture.h"

struct Vertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
	glm::vec3 Tangent;
};

class Model
{
public:
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	Texture texture; // TODO: should be replaced with a material class?
	
	Model(std::vector<Vertex> vertices, std::vector<unsigned int> indices)
	{
		this->vertices = vertices;
		this->indices = indices;

		computeTangents();
		setUpModel();
	}

	~Model()
	{
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
		glDeleteBuffers(1, &EBO);
	}

	void setTexture(Texture& texture)
	{
		this->texture = texture;
	}

	// TODO: move drawing/rendering into a renderer class
	void draw(Shader& shader, glm::vec3 position = glm::vec3(0.0f), glm::vec3 scale = glm::vec3(1.0f))
	{
		shader.use();
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, position);
		
		model = glm::scale(model, scale);

		shader.setMat4("model", model);

		glActiveTexture(GL_TEXTURE0);
		texture.bind();

		glBindVertexArray(this->VAO);
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}

	static Model* loadModel(std::string modelPath);

private:
	unsigned int VAO, VBO, EBO;

	void setUpModel()
	{
		// Generate the buffer objects
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s)
		glBindVertexArray(VAO);

		// copy vertices array into buffer
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

		// copy indices array into buffer
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

		// set vertex attribute pointers so that OpenGL understands the format of our vertex data
		// Position attribute
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

		// Normal attribute
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));

		// Texture coord attribute
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

		// FOR TANGENT AND BI TANGENT
		// Tangent attribute
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));

		glBindVertexArray(0);
	}

	void computeTangents()
	{
		for (unsigned int i = 0; i < indices.size(); i += 3)
		{
			Vertex& v0 = vertices[indices[i]];
			Vertex& v1 = vertices[indices[i+1]];
			Vertex& v2 = vertices[indices[i+2]];

			glm::vec3 Edge1 = v1.Position - v0.Position;
			glm::vec3 Edge2 = v2.Position - v0.Position;

			glm::vec2 DeltaUV1 = v1.TexCoords - v0.TexCoords;
			glm::vec2 DeltaUV2 = v2.TexCoords - v0.TexCoords;

			float f = 1.0f / (DeltaUV1.x * DeltaUV2.y - DeltaUV2.x * DeltaUV1.y);

			glm::vec3 tangent, bitangent;

			tangent.x = f * (DeltaUV2.y * Edge1.x - DeltaUV1.y * Edge2.x);
			tangent.y = f * (DeltaUV2.y * Edge1.y - DeltaUV1.y * Edge2.y);
			tangent.z = f * (DeltaUV2.y * Edge1.z - DeltaUV1.y * Edge2.z);

			bitangent.x = f * (-DeltaUV2.x * Edge1.x + DeltaUV1.x * Edge2.x);
			bitangent.y = f * (-DeltaUV2.x * Edge1.y + DeltaUV1.x * Edge2.y);
			bitangent.z = f * (-DeltaUV2.x * Edge1.z + DeltaUV1.x * Edge2.z);

			v0.Tangent += tangent;
			v1.Tangent += tangent;
			v2.Tangent += tangent;
		}

		for (const auto& vertex : vertices)
		{
			glm::normalize(vertex.Tangent);
		}
	}
};
