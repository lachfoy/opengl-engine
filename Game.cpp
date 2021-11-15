#include "Game.h"

Game::Game(unsigned int width, unsigned int height)
	: State{ GAME_ACTIVE }, Keys{ }, Width{ width }, Height{ height }
{
}

Game::~Game()
{
	delete Renderer;
	delete testModel;
}

void Game::init()
{
	// load shaders
	ResourceManager::loadShader("shaders/sprite.vert", "shaders/sprite.frag", nullptr, "sprite");
	ResourceManager::loadShader("shaders/model.vert", "shaders/model.frag", nullptr, "model");

	// configure shaders
	glm::mat4 projection;
	projection = glm::ortho(0.0f, static_cast<float>(this->Width), static_cast<float>(this->Height), 0.0f, -1.0f, 1.0f);
	ResourceManager::getShader("sprite").use().setInt("image", 0);
	ResourceManager::getShader("sprite").use().setMat4("projection", projection);

	glm::mat4 view = glm::mat4(1.0f);
	view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f)); // push camera back a lil
	projection = glm::perspective(glm::radians(45.0f), static_cast<float>(this->Width) / static_cast<float>(this->Height), 0.1f, 100.0f);
	ResourceManager::getShader("model").use().setInt("diffuse", 0);
	ResourceManager::getShader("model").use().setMat4("projection", projection);

	// set render-specific controls
	Renderer = new SpriteRenderer(ResourceManager::getShader("sprite"));

	// load textures
	ResourceManager::loadTexture("images/test_texture.png", true, "test_texture");

	// load models
	testModel = Model::loadModel("models/cube.obj");
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
	glEnable(GL_DEPTH_TEST);

	testModel->draw(ResourceManager::getShader("model").use());
}
