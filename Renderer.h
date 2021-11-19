#pragma once

#include "Mesh.h"
#include "Material.h"
#include "Camera.h"

#include <vector>

struct RenderCommand
{
	Mesh* mesh;
	Material* material;
	glm::mat4 transform;
};

class Renderer
{
public:
	Renderer();
	~Renderer();

	void init(unsigned int renderWidth, unsigned int renderHeight);

	void push(Mesh* mesh, Material* material, glm::mat4 transform);
	void render(Camera* camera = nullptr);

	//void addLight(DirectionalLight* light)?

private:
	unsigned int mRenderWidth, mRenderHeight;
	std::vector<RenderCommand> mRenderCommands;

	void renderMesh(Mesh* mesh, Shader* shader);
};