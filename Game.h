#pragma once

#include "ResourceManager.h"
#include "SpriteRenderer.h"
#include "Model.h"

enum GameState
{
	GAME_ACTIVE,
	GAME_MENU,
	GAME_WIN
};

class Game
{
public:
	GameState State;
	bool Keys[1024];
	bool KeysProcessed[1024];
	unsigned int Width, Height;
	SpriteRenderer* Renderer;

	Model* testModel; // delete this lol

	Game(unsigned int width, unsigned int height);
	~Game();

	void init();
	void processInput(float deltaTime);
	void update(float deltaTime);
	void render();
};