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
#include "PowerUp.h"

enum Direction {
	UP,
	RIGHT,
	DOWN,
	LEFT
};

enum GameState{
	GAME_ACTIVE,
	GAME_MENU,
	GAME_WIN
};


// Defines a Collision typedef that represents collision data
// <collision?, what direction?, difference vector center - closest point>
typedef std::tuple<bool, Direction, glm::vec2> Collision; 


const glm::vec2 PLAYER_SIZE(100.0f, 20.0f);

const float PLAYER_VELOCITY(500.0f);

class Game
{
	public:
		GameState State;
		bool Keys[1024];
		unsigned int Width, Height;

		std::vector<GameLevel> Levels;
		std::vector<PowerUp> PowerUps;
		unsigned int Level;

		Game(unsigned int width, unsigned int height);
		~Game();

		void Init();

		void ProcessInput(float dt);
		void Update(float dt);
		void Render();
		void DoCollisions();

		// reset
		void ResetLevel();
		void ResetPlayer();

		// Power up
		void SpawnPowerUps(GameObj& block);
		void UpdatePowerUps(float dt);
		

private:
	void loadTexture();
	void loadLevels();
	Collision CheckCollision(BallObject& one, GameObj& two);
	bool CheckCollision(GameObj& one, GameObj& two);
	
};

#endif
