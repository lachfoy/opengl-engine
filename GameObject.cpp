#include "GameObject.h"

GameObject::GameObject()
{
	this->transform.Position = glm::vec3(0.0f, 0.0f, 0.0f);
	this->transform.Scale = glm::vec3(1.0f, 1.0f, 1.0f);
	this->transform.Rotation = glm::vec3(0.0f, 0.0f, 0.0f);
}