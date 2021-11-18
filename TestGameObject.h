#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "GameObject.h"
#include "Material.h"
#include "Mesh.h"
#include "ModelRenderer.h"

class TestGameObject : public GameObject
{
public:
	Mesh* mesh;
	Material* material;
	ModelRenderer* renderer;
	glm::vec3 velocity;

	TestGameObject();

	void draw() override;
};