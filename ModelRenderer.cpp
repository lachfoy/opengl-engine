#include "ModelRenderer.h"

ModelRenderer::ModelRenderer(Shader& shader)
{
	this->shader = shader;
}

ModelRenderer::~ModelRenderer()
{

}

void ModelRenderer::drawModel(Mesh& mesh, glm::vec3 position, glm::vec3 scale)
{
	this->shader.use();
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, position);

	model = glm::scale(model, scale);

	this->shader.setMat4("model", model);

	//glActiveTexture(GL_TEXTURE0);
	//p_model.texture.bind();

	glBindVertexArray(mesh.VAO);
	glDrawElements(GL_TRIANGLES, mesh.indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}
