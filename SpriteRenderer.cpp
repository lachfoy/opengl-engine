#include "SpriteRenderer.h"

SpriteRenderer::SpriteRenderer(Shader& shader)
{
	this->shader = shader;
	this->initRenderData();
}

SpriteRenderer::~SpriteRenderer()
{
	glDeleteVertexArrays(1, &this->quadVAO);
}

void SpriteRenderer::drawSprite(Texture& texture, glm::vec2 position, glm::vec2 size, float rotate, glm::vec3 color)
{
	this->shader.use();
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(position, 0.0f));

	// To rotate around the center instead of the top left corner, translate by half its size then translate back
	model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f));
	model = glm::rotate(model, glm::radians(rotate), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f));

	model = glm::scale(model, glm::vec3(size, 1.0f));

	this->shader.setMat4("model", model);
	this->shader.setVec3("spriteColor", color);

	glActiveTexture(GL_TEXTURE0);
	texture.bind();

	glBindVertexArray(this->quadVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}

// TODO: use GL_TRIANGLE_STRIP to save some verts
void SpriteRenderer::initRenderData()
{
	// Creates the vertices and configures the VAO and VBO for quad rendering
	unsigned int VBO;

	float vertices[] = {
		// pos		// tex
		0.0f, 1.0f,	0.0f, 1.0f,
		1.0f, 0.0f,	1.0f, 0.0f,
		0.0f, 0.0f,	0.0f, 0.0f,

		0.0f, 1.0f,	0.0f, 1.0f,
		1.0f, 1.0f,	1.0f, 1.0f,
		1.0f, 0.0f,	1.0f, 0.0f
	};

	glGenVertexArrays(1, &this->quadVAO);
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindVertexArray(this->quadVAO);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}