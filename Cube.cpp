#include "Cube.h"

Cube::Cube()
{
	this->vertices = std::vector<Vertex> {
		// positions						// normals						// tex coords
		{ glm::vec3(-0.5f, -0.5f, -0.5f),  glm::vec3( 0.0f,  0.0f, -1.0f), glm::vec2(0.0f, 0.0f) },
		{ glm::vec3( 0.5f,  0.5f, -0.5f),  glm::vec3( 0.0f,  0.0f, -1.0f), glm::vec2(1.0f, 1.0f) },
		{ glm::vec3( 0.5f, -0.5f, -0.5f),  glm::vec3( 0.0f,  0.0f, -1.0f), glm::vec2(1.0f, 0.0f) },
		{ glm::vec3( 0.5f,  0.5f, -0.5f),  glm::vec3( 0.0f,  0.0f, -1.0f), glm::vec2(1.0f, 1.0f) },
		{ glm::vec3(-0.5f, -0.5f, -0.5f),  glm::vec3( 0.0f,  0.0f, -1.0f), glm::vec2(0.0f, 0.0f) },
		{ glm::vec3(-0.5f,  0.5f, -0.5f),  glm::vec3( 0.0f,  0.0f, -1.0f), glm::vec2(0.0f, 1.0f) },

		{ glm::vec3(-0.5f, -0.5f,  0.5f),  glm::vec3( 0.0f,  0.0f,  1.0f), glm::vec2(0.0f, 0.0f) },
		{ glm::vec3( 0.5f, -0.5f,  0.5f),  glm::vec3( 0.0f,  0.0f,  1.0f), glm::vec2(1.0f, 0.0f) },
		{ glm::vec3( 0.5f,  0.5f,  0.5f),  glm::vec3( 0.0f,  0.0f,  1.0f), glm::vec2(1.0f, 1.0f) },
		{ glm::vec3( 0.5f,  0.5f,  0.5f),  glm::vec3( 0.0f,  0.0f,  1.0f), glm::vec2(1.0f, 1.0f) },
		{ glm::vec3(-0.5f,  0.5f,  0.5f),  glm::vec3( 0.0f,  0.0f,  1.0f), glm::vec2(0.0f, 1.0f) },
		{ glm::vec3(-0.5f, -0.5f,  0.5f),  glm::vec3( 0.0f,  0.0f,  1.0f), glm::vec2(0.0f, 0.0f) },

		{ glm::vec3(-0.5f,  0.5f,  0.5f),  glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec2(1.0f, 0.0f) },
		{ glm::vec3(-0.5f,  0.5f, -0.5f),  glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec2(1.0f, 1.0f) },
		{ glm::vec3(-0.5f, -0.5f, -0.5f),  glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec2(0.0f, 1.0f) },
		{ glm::vec3(-0.5f, -0.5f, -0.5f),  glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec2(0.0f, 1.0f) },
		{ glm::vec3(-0.5f, -0.5f,  0.5f),  glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec2(0.0f, 0.0f) },
		{ glm::vec3(-0.5f,  0.5f,  0.5f),  glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec2(1.0f, 0.0f) },

		{ glm::vec3( 0.5f,  0.5f,  0.5f),  glm::vec3( 1.0f,  0.0f,  0.0f), glm::vec2(1.0f, 0.0f) },
		{ glm::vec3( 0.5f, -0.5f, -0.5f),  glm::vec3( 1.0f,  0.0f,  0.0f), glm::vec2(0.0f, 1.0f) },
		{ glm::vec3( 0.5f,  0.5f, -0.5f),  glm::vec3( 1.0f,  0.0f,  0.0f), glm::vec2(1.0f, 1.0f) },
		{ glm::vec3( 0.5f, -0.5f, -0.5f),  glm::vec3( 1.0f,  0.0f,  0.0f), glm::vec2(0.0f, 1.0f) },
		{ glm::vec3( 0.5f,  0.5f,  0.5f),  glm::vec3( 1.0f,  0.0f,  0.0f), glm::vec2(1.0f, 0.0f) },
		{ glm::vec3( 0.5f, -0.5f,  0.5f),  glm::vec3( 1.0f,  0.0f,  0.0f), glm::vec2(0.0f, 0.0f) },

		{ glm::vec3(-0.5f, -0.5f, -0.5f),  glm::vec3( 0.0f, -1.0f,  0.0f), glm::vec2(0.0f, 1.0f) },
		{ glm::vec3( 0.5f, -0.5f, -0.5f),  glm::vec3( 0.0f, -1.0f,  0.0f), glm::vec2(1.0f, 1.0f) },
		{ glm::vec3( 0.5f, -0.5f,  0.5f),  glm::vec3( 0.0f, -1.0f,  0.0f), glm::vec2(1.0f, 0.0f) },
		{ glm::vec3( 0.5f, -0.5f,  0.5f),  glm::vec3( 0.0f, -1.0f,  0.0f), glm::vec2(1.0f, 0.0f) },
		{ glm::vec3(-0.5f, -0.5f,  0.5f),  glm::vec3( 0.0f, -1.0f,  0.0f), glm::vec2(0.0f, 0.0f) },
		{ glm::vec3(-0.5f, -0.5f, -0.5f),  glm::vec3( 0.0f, -1.0f,  0.0f), glm::vec2(0.0f, 1.0f) },

		{ glm::vec3(-0.5f,  0.5f, -0.5f),  glm::vec3( 0.0f,  1.0f,  0.0f), glm::vec2(0.0f, 1.0f) },
		{ glm::vec3( 0.5f,  0.5f,  0.5f),  glm::vec3( 0.0f,  1.0f,  0.0f), glm::vec2(1.0f, 0.0f) },
		{ glm::vec3( 0.5f,  0.5f, -0.5f),  glm::vec3( 0.0f,  1.0f,  0.0f), glm::vec2(1.0f, 1.0f) },
		{ glm::vec3( 0.5f,  0.5f,  0.5f),  glm::vec3( 0.0f,  1.0f,  0.0f), glm::vec2(1.0f, 0.0f) },
		{ glm::vec3(-0.5f,  0.5f, -0.5f),  glm::vec3( 0.0f,  1.0f,  0.0f), glm::vec2(0.0f, 1.0f) },
		{ glm::vec3(-0.5f,  0.5f,  0.5f),  glm::vec3( 0.0f,  1.0f,  0.0f), glm::vec2(0.0f, 0.0f) },
	};

	this->topology = TRIANGLES;

	setUpMesh();
}