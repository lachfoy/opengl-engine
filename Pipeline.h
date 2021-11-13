#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "Camera.h"

struct Transform
{
	glm::vec3 scale;
	glm::vec3 rotation;
	glm::vec3 position;

	Transform()
	{
		scale = glm::vec3(1.0f, 1.0f, 1.0f);
		rotation = glm::vec3(0.0f, 0.0f, 0.0f);
		position = glm::vec3(0.0f, 0.0f, 0.0f);
	}
};

class Pipeline
{
public:
	Pipeline()
	{
		scale = glm::vec3(1.0f, 1.0f, 1.0f);
		rotateInfo = glm::vec3(0.0f, 0.0f, 0.0f);
		worldPosition = glm::vec3(0.0f, 0.0f, 0.0f);
	}

	//void setCamera();

private:
	glm::vec3 scale;
	glm::vec3 rotateInfo;
	glm::vec3 worldPosition;
};