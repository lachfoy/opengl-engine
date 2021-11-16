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
	projection = glm::perspective(glm::radians(75.0f), static_cast<float>(this->Width) / static_cast<float>(this->Height), 0.1f, 100.0f);
	ResourceManager::getShader("model").use().setInt("diffuse", 0);
	ResourceManager::getShader("model").use().setMat4("projection", projection);
	ResourceManager::getShader("model").use().setMat4("view", view);

	// set render-specific controls
	Renderer = new SpriteRenderer(ResourceManager::getShader("sprite"));
	modelRenderer = new ModelRenderer(ResourceManager::getShader("model"));

	// load textures
	ResourceManager::loadTexture("images/test_texture.png", true, "test_texture");

	// load models
	ResourceManager::loadModel("models/cube.obj", "cube");

	// set texture for model
	ResourceManager::getModel("cube").setTexture(ResourceManager::getTexture("test_texture"));
}

void Game::processInput(float deltaTime)
{
}

void Game::update(float deltaTime)
{
}

void Game::render()
{
	glDisable(GL_DEPTH_TEST); // Disable depth testing for sprite rendering
	Renderer->drawSprite(ResourceManager::getTexture("test_texture"), glm::vec2(200.0f, 100.0f), glm::vec2(80.0f, 100.0f));
	//glEnable(GL_DEPTH_TEST);

	modelRenderer->drawModel(ResourceManager::getModel("cube"), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f));
}
