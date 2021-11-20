#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Mesh.h"
#include "Camera.h"
#include "Material.h"

class MeshRenderer
{
public:
	MeshRenderer();
	MeshRenderer(Camera* camera);
	~MeshRenderer();

	void setCamera(Camera* camera);
	Camera* getCamera();

	void drawMesh(Mesh* mesh, Shader* material, glm::mat4 transform, Camera* camera = nullptr);

private:
	Camera* mCamera;
};