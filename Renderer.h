#pragma once

#include "Shader.h"

class Mesh;

class Renderer
{
public:
	Renderer();
	~Renderer();

	void init();
	void blit();

private:
	void renderMesh(Mesh* mesh, Shader* shader);

};