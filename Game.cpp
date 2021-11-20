#include "Game.h"

Game::Game(unsigned int width, unsigned int height)
	: State{ GAME_ACTIVE }, Keys{ }, Width{ width }, Height{ height }
{
}

Game::~Game()
{
	delete spriteRenderer;
	delete mMeshRenderer;
	delete mCamera;
}

void Game::init()
{
	// load shaders
	ResourceManager::loadShader("shaders/sprite.vert", "shaders/sprite.frag", nullptr, "sprite");
	ResourceManager::loadShader("shaders/default.vert", "shaders/default.frag", nullptr, "default");

	// configure shaders
	glm::mat4 projection = glm::mat4(1.0f);
	projection = glm::ortho(0.0f, static_cast<float>(this->Width), static_cast<float>(this->Height), 0.0f, -1.0f, 1.0f);
	ResourceManager::getShader("sprite").use().setInt("image", 0);
	ResourceManager::getShader("sprite").use().setMat4("projection", projection);

	glm::mat4 view = glm::mat4(1.0f);
	projection = glm::mat4(1.0f);
	view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f)); // push view back a bit
	projection = glm::perspective(glm::radians(45.0f), static_cast<float>(this->Width) / static_cast<float>(this->Height), 0.1f, 100.0f);
	ResourceManager::getShader("default").use().setMat4("projection", projection);
	ResourceManager::getShader("default").use().setInt("diffuse", 0);
	ResourceManager::getShader("default").use().setMat4("view", view);

	// set directional light uniform properties
	ResourceManager::getShader("default").use().setVec3("light.direction", glm::vec3(-0.2f, -1.0f, -0.3f));
	ResourceManager::getShader("default").use().setVec3("light.ambient", glm::vec3(0.1f, 0.1f, 0.1f));
	ResourceManager::getShader("default").use().setVec3("light.diffuse", glm::vec3(0.8f, 0.8f, 0.8f));
	ResourceManager::getShader("default").use().setVec3("light.specular", glm::vec3(1.0f, 1.0f, 1.0f));

	// camera
	mCamera = new Camera(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	mCamera->setPerspective(45.0f, static_cast<float>(this->Width) / static_cast<float>(this->Height), 0.1f, 100.0f);

	// materials
	mDefaultMat = std::make_shared<Material>(std::make_shared<Shader>(ResourceManager::getShader("default")));
	mDefaultMat->setVec3("objectColor", glm::vec3(0.0f, 1.0f, 1.0f));
	mDefaultMat->updateUniforms();
	mDefaultMat->getShaderPointer()->setMat4("projection", mCamera->Projection);
	mDefaultMat->getShaderPointer()->setMat4("view", mCamera->View);
	mDefaultMat->getShaderPointer()->setVec3("viewPos", mCamera->Position);

	// set render-specific controls
	spriteRenderer = new SpriteRenderer(ResourceManager::getShader("sprite"));
	mMeshRenderer = new MeshRenderer();

	// load textures
	ResourceManager::loadTexture("images/jinx.png", false, "jinx");

	// load meshes
	ResourceManager::loadMesh("models/bunny_normals_smooth.obj", "bunny");
}

void Game::processInput(float deltaTime)
{
}

void Game::update(float deltaTime)
{
}

void Game::render()
{
	//glDisable(GL_DEPTH_TEST);
	//modelRenderer->drawModel(ResourceManager::getMesh("bunny"), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f));

	glm::mat4 transform = glm::mat4(1.0f);
	transform = glm::translate(transform, glm::vec3(0.0f, 0.0f, 0.0f));
	mMeshRenderer->drawMesh(&ResourceManager::getMesh("bunny"), mDefaultMat->getShaderPointer().get(), transform, mCamera);

	//glDisable(GL_DEPTH_TEST); // Disable depth testing for sprite rendering
	//spriteRenderer->drawSprite(ResourceManager::getTexture("jinx"), glm::vec2(100.0f, 100.0f), glm::vec2(100.0f, 150.0f));
	//glEnable(GL_DEPTH_TEST);
}