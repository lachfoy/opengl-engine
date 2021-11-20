#pragma once

#include "ResourceManager.h"
#include "SpriteRenderer.h"
#include "Mesh.h"
#include "MeshRenderer.h"
#include "Material.h"

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
	SpriteRenderer* spriteRenderer;
	MeshRenderer* mMeshRenderer;
	Material* mDefaultMat;
	Camera* mCamera;

	Game(unsigned int width, unsigned int height);
	~Game();

	void init();
	void processInput(float deltaTime);
	void update(float deltaTime);
	void render();
};