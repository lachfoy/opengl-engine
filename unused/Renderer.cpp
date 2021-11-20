// NOTE: this class is not used as of right now
#include "Renderer.h"

Renderer::Renderer()
{
}

Renderer::~Renderer()
{

}

void Renderer::init(unsigned int renderWidth, unsigned int renderHeight)
{
	// set default openGL states?
	mRenderWidth = renderWidth;
	mRenderHeight = renderHeight;
}

void Renderer::push(Mesh* mesh, Material* material, glm::mat4 transform)
{
	RenderCommand renderCommand{};
	renderCommand.mesh = mesh;
	renderCommand.material = material;
	renderCommand.transform = transform;
	mRenderCommands.push_back(renderCommand);
}

void Renderer::render(Camera* camera)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// update global UBOs? not sure how to do that

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glViewport(0, 0, mRenderWidth, mRenderWidth);
	camera->setPerspective(camera->Fov, (float)mRenderWidth / (float)mRenderHeight, 0.1f, 100.0f);

	// render all pushed geometry
	
	for (unsigned int i = 0; i < mRenderCommands.size(); ++i)
	{
		Material* material = mRenderCommands[i].material;
		Mesh* mesh = mRenderCommands[i].mesh;

		material->getShader()->use().setMat4("projection", camera->Projection);
		material->getShader()->use().setMat4("view", camera->View);
		//material->getShader()->use().setVec3("viewPos", camera->Position); will be needed for specular
		material->getShader()->use().setMat4("model", mRenderCommands[i].transform);

		// bind sampler objects here

		material->updateUniforms();

		// finally render the mesh
		renderMesh(mesh, material->getShader());
	}
}

void Renderer::renderMesh(Mesh* mesh, Shader* shader)
{
	glBindVertexArray(mesh->VAO);
	if (mesh->indices.size() > 0)
	{
		glDrawElements(mesh->topology == TRIANGLE_STRIP ? GL_TRIANGLE_STRIP : GL_TRIANGLES, mesh->indices.size(), GL_UNSIGNED_INT, 0);
	}
	else
	{
		glDrawArrays(mesh->topology == TRIANGLE_STRIP ? GL_TRIANGLE_STRIP : GL_TRIANGLES, 0, mesh->vertices.size());
	}
	glBindVertexArray(0);
}
