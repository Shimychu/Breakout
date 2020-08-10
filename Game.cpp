#include "Game.h"

#include <iostream>

#include "ResourceManager.h"
#include "Sprite.h"

Sprite* Renderer;
GameObj* Player;

Game::Game(unsigned int width, unsigned int height)
	: State(GAME_ACTIVE), Keys(), Width(width), Height(height)
{

}

Game::~Game()
{
	delete Renderer;
	delete Player;
}


void Game::Init()
{
	// Load Shaders
	ResourceManager::LoadShader("sprite.vs", "sprite.fs", nullptr, "sprite");

	// Configure Shaders
	glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(this->Width),static_cast<float>(this->Height), 0.0f, -1.0f, 1.0f);

	ResourceManager::GetShader("sprite").Use().SetInteger("image", 0);
	ResourceManager::GetShader("sprite").SetMatrix4("projection", projection);

	// Set render specific controls
	Renderer = new Sprite(ResourceManager::GetShader("sprite"));

	// Load Texture
	loadTexture();

	// Load Levels
	loadLevels();
	
}

void Game::Update(float dt)
{

}

void Game::ProcessInput(float dt)
{
	if (this->State == GAME_ACTIVE)
	{
		float velocity = PLAYER_VELOCITY * dt;
		if (this->Keys[GLFW_KEY_A])
		{
			if (Player->position.x >= 0.0f)
				Player->position.x -= velocity;
		}
		if (this->Keys[GLFW_KEY_D])
		{
			if (Player->position.x <= this->Width - Player->size.x)
				Player->position.x += velocity;
		}
	}
}

void Game::Render()
{
	if (this->State == GAME_ACTIVE)
	{
		// draw background
		Renderer->DrawSprite(ResourceManager::GetTexture("background"),
			glm::vec2(0.0f, 0.0f), glm::vec2(this->Width, this->Height), 0.0f
		);

		// draw level
		this->Levels[this->Level].Draw(*Renderer);

		// draw player
		Player->Draw(*Renderer);
	}
}

void Game::loadTexture()
{
	ResourceManager::LoadTexture("F:\\Repo\\Breakout\\Breakout\\Images\\background.jpg", false, "background");
	ResourceManager::LoadTexture("F:\\Repo\\Breakout\\Breakout\\Images\\awesomeface.png", true, "face");
	ResourceManager::LoadTexture("F:\\Repo\\Breakout\\Breakout\\Images\\block.png", false, "block");
	ResourceManager::LoadTexture("F:\\Repo\\Breakout\\Breakout\\Images\\block_solid.png", false, "block_solid");
	ResourceManager::LoadTexture("F:\\Repo\\Breakout\\Breakout\\Images\\paddle.png", true, "paddle");
	glm::vec2 playerPos = glm::vec2(
		this->Width / 2.0f - PLAYER_SIZE.x / 2.0f,
		this->Height - PLAYER_SIZE.y
	);
	Player = new GameObj(playerPos, PLAYER_SIZE, ResourceManager::GetTexture("paddle"));
}

void Game::loadLevels()
{
	GameLevel one; 
	GameLevel two; 
	GameLevel three; 
	GameLevel four; 

	one.Load("F:\\Repo\\Breakout\\Breakout\\levels\\one.lvl", this->Width, this->Height / 2);
	two.Load("F:\\Repo\\Breakout\\Breakout\\levels\\two.lvl", this->Width, this->Height / 2);
	three.Load("F:\\Repo\\Breakout\\Breakout\\levels\\three.lvl", this->Width, this->Height / 2);
	four.Load("F:\\Repo\\Breakout\\Breakout\\levels\\four.lvl", this->Width, this->Height / 2);

	this->Levels.push_back(one);
	this->Levels.push_back(two);
	this->Levels.push_back(three);
	this->Levels.push_back(four);
	this->Level = 0;
}