#include "MeshRenderer.h"

MeshRenderer::MeshRenderer()
{
}

MeshRenderer::MeshRenderer(Camera* camera)
{
	mCamera = camera;
}

MeshRenderer::~MeshRenderer()
{
	delete mCamera;
}

void MeshRenderer::setCamera(Camera* camera)
{
}

Camera* MeshRenderer::getCamera()
{
	return nullptr;
}

void MeshRenderer::drawMesh(Mesh* mesh, Shader* material, glm::mat4 transform, Camera* camera)
{
	material->Use();
	material->setMat4("model", transform);

	//glActiveTexture(GL_TEXTURE0);
	//p_model.texture.bind();
	material->Use();
	glBindVertexArray(mesh->VAO);
	if (mesh->indices.size() > 0)
	{
		glDrawElements(GL_TRIANGLES, mesh->indices.size(), GL_UNSIGNED_INT, 0);
	}
	else
	{
		glDrawArrays(GL_TRIANGLES, 0, mesh->vertices.size());
	}
	glBindVertexArray(0);
}
