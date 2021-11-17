#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Texture.h"
#include "Shader.h"
#include "Mesh.h"

class ModelRenderer
{
public:
	ModelRenderer(Shader& shader);
	~ModelRenderer();

	void drawModel(Mesh& mesh, glm::vec3 position, glm::vec3 scale);

private:
	Shader shader;
};