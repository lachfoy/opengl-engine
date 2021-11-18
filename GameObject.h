#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "Texture.h"
#include "SpriteRenderer.h"

struct Transform
{
	glm::vec3 Position;
	glm::vec3 Scale;
	glm::vec3 Rotation;
};

class GameObject
{
public:
	Transform transform;
	GameObject();

	virtual void draw() {}
};