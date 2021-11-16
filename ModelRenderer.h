#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Texture.h"
#include "Shader.h"
#include "Model.h"

class ModelRenderer
{
public:
	ModelRenderer(Shader& shader);
	~ModelRenderer();

	void drawModel(Model& p_model, glm::vec3 position, glm::vec3 scale);

private:
	Shader modelShader;
};