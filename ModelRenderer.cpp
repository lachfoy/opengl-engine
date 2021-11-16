#include "ModelRenderer.h"

ModelRenderer::ModelRenderer(Shader& shader)
{
	this->modelShader = shader;
}

ModelRenderer::~ModelRenderer()
{

}

void ModelRenderer::drawModel(Model& p_model, glm::vec3 position, glm::vec3 scale)
{
	this->modelShader.use();
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, position);

	model = glm::scale(model, scale);

	this->modelShader.setMat4("model", model);

	//glActiveTexture(GL_TEXTURE0);
	//p_model.texture.bind();

	glBindVertexArray(p_model.VAO);
	glDrawElements(GL_TRIANGLES, p_model.indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}
