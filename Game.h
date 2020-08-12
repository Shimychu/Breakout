#pragma once

#ifndef GAME_H
#define GAME_H



#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>

#include "GameLevel.h"

#include "BallObject.h"


enum GameState{
	GAME_ACTIVE,
	GAME_MENU,
	GAME_WIN
};

const glm::vec2 PLAYER_SIZE(100.0f, 20.0f);

const float PLAYER_VELOCITY(500.0f);

class Game
{
	public:
		GameState State;
		bool Keys[1024];
		unsigned int Width, Height;

		std::vector<GameLevel> Levels;
		unsigned int Level;

		Game(unsigned int width, unsigned int height);
		~Game();

		void Init();

		void ProcessInput(float dt);
		void Update(float dt);
		void Render();
		void DoCollisions();
		

private:
	void loadTexture();
	void loadLevels();
	bool CheckCollision(BallObject& one, GameObj& two);
	
};

#endif
