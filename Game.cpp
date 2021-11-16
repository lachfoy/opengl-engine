#include "Game.h"

Game::Game(unsigned int width, unsigned int height)
	: State{ GAME_ACTIVE }, Keys{ }, Width{ width }, Height{ height }
{
}

Game::~Game()
{
	delete Renderer;
	delete modelRenderer;
}

void Game::init()
{
	// load shaders
	ResourceManager::loadShader("shaders/sprite.vert", "shaders/sprite.frag", nullptr, "sprite");
	ResourceManager::loadShader("shaders/model.vert", "shaders/model.frag", nullptr, "model");

	// configure shaders
	glm::mat4 projection = glm::mat4(1.0f);
	projection = glm::ortho(0.0f, static_cast<float>(this->Width), static_cast<float>(this->Height), 0.0f, -1.0f, 1.0f);
	ResourceManager::getShader("sprite").use().setInt("image", 0);
	ResourceManager::getShader("sprite").use().setMat4("projection", projection);

	glm::mat4 view = glm::mat4(1.0f);
	projection = glm::mat4(1.0f);
	view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f)); // push view back a bit
	projection = glm::perspective(glm::radians(45.0f), static_cast<float>(this->Width) / static_cast<float>(this->Height), 0.1f, 100.0f);
	ResourceManager::getShader("model").use().setMat4("projection", projection);
	ResourceManager::getShader("model").use().setInt("diffuse", 0);
	ResourceManager::getShader("model").use().setMat4("view", view);

	// set directional light uniform properties
	ResourceManager::getShader("model").use().setVec3("light.direction", glm::vec3(-0.2f, -1.0f, -0.3f));
	ResourceManager::getShader("model").use().setVec3("light.ambient", glm::vec3(0.1f, 0.1f, 0.1f));
	ResourceManager::getShader("model").use().setVec3("light.diffuse", glm::vec3(0.8f, 0.8f, 0.8f));
	ResourceManager::getShader("model").use().setVec3("light.specular", glm::vec3(1.0f, 1.0f, 1.0f));

	// set render-specific controls
	Renderer = new SpriteRenderer(ResourceManager::getShader("sprite"));
	modelRenderer = new ModelRenderer(ResourceManager::getShader("model"));

	// load textures
	ResourceManager::loadTexture("images/jinx.png", false, "jinx");

	// load models
	ResourceManager::loadModel("models/bunny_normals_smooth.obj", "bunny");

	// set texture for model
	ResourceManager::getModel("bunny").setTexture(ResourceManager::getTexture("jinx"));

	// set up ground plane TODO: move this?
	setUpGroundPlane();
}

void Game::processInput(float deltaTime)
{
}

void Game::update(float deltaTime)
{
}

void Game::render()
{
	glDisable(GL_DEPTH_TEST);
	drawGroundPlane(ResourceManager::getShader("model"));
	modelRenderer->drawModel(ResourceManager::getModel("bunny"), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f));

	//glDisable(GL_DEPTH_TEST); // Disable depth testing for sprite rendering
	//Renderer->drawSprite(ResourceManager::getTexture("jinx"), glm::vec2(200.0f, 100.0f), glm::vec2(80.0f, 100.0f));
	//glEnable(GL_DEPTH_TEST);
}

void Game::drawGroundPlane(Shader& shader)
{
	shader.use();
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));

	shader.setMat4("model", model);

	glBindVertexArray(this->planeVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}

void Game::setUpGroundPlane()
{
	unsigned int planeVBO;
	float planeVertices[] = {
		// positions            // normals         // texcoords
		 25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,  25.0f,  0.0f,
		-25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,   0.0f,  0.0f,
		-25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,   0.0f, 25.0f,

		 25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,  25.0f,  0.0f,
		-25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,   0.0f, 25.0f,
		 25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,  25.0f, 25.0f
	};

	glGenVertexArrays(1, &this->planeVAO);
	glGenBuffers(1, &planeVBO);

	glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), planeVertices, GL_STATIC_DRAW);

	glBindVertexArray(this->planeVAO);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glBindVertexArray(0);
}