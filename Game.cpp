#include "Game.h"

#include <iostream>
#include <algorithm>

#include "ResourceManager.h"
#include "Sprite.h"
#include "GameObj.h"
#include "ParticleGenerator.h"

// Game setting inital ball speed
const glm::vec2 INITIAL_BALL_VELOCITY(100.0f, -350.0f);

// Radius of ball object
const float BALL_RADIUS = 12.5f;

Sprite* Renderer;
GameObj* Player;
BallObject* Ball;
ParticleGenerator* Particles;

Game::Game(unsigned int width, unsigned int height)
	: State(GAME_ACTIVE), Keys(), Width(width), Height(height)
{

}

Game::~Game()
{
	delete Renderer;
	delete Player;
	delete Ball;
	delete Particles;
}


void Game::Init()
{
	// Load Shaders
	ResourceManager::LoadShader("sprite.vs", "sprite.fs", nullptr, "sprite");
	ResourceManager::LoadShader("particle.vs", "particle.fs", nullptr, "particle");

	// Configure Shaders
	glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(this->Width),static_cast<float>(this->Height), 0.0f, -1.0f, 1.0f);

	ResourceManager::GetShader("sprite").Use().SetInteger("image", 0);
	ResourceManager::GetShader("sprite").SetMatrix4("projection", projection);
	ResourceManager::GetShader("particle").Use().SetInteger("sprite", 0);
	ResourceManager::GetShader("particle").SetMatrix4("projection", projection);


	// Set render specific controls
	Renderer = new Sprite(ResourceManager::GetShader("sprite"));

	// Load Texture
	loadTexture();

	Particles = new ParticleGenerator(
		ResourceManager::GetShader("particle"),
		ResourceManager::GetTexture("particle"),
		400
	);
	// Load Levels
	loadLevels();

	glm::vec2 playerPos = glm::vec2(
		this->Width / 2.0f - PLAYER_SIZE.x / 2.0f,
		this->Height - PLAYER_SIZE.y
	);
	Player = new GameObj(playerPos, PLAYER_SIZE, ResourceManager::GetTexture("paddle"));
	
	glm::vec2 ballPos = playerPos + glm::vec2(PLAYER_SIZE.x / 2.0f - BALL_RADIUS, -BALL_RADIUS * 2.0f);

	Ball = new BallObject(ballPos, BALL_RADIUS, INITIAL_BALL_VELOCITY, ResourceManager::GetTexture("face"));


}

void Game::Update(float dt)
{
	Ball->Move(dt, this->Width);
	this->DoCollisions();
	//if(!Ball->stuck)
		Particles->Update(dt, *Ball, 2, glm::vec2(Ball->radius / 2.0f));
	if (Ball->position.y >= this->Height)
	{
		this->ResetLevel();
		this->ResetPlayer();
	}
}

void Game::ResetLevel()
{
	if (this->Level == 0)
		this->Levels[0].Load("F:\\Repo\\Breakout\\Breakout\\levels\\one.lvl", this->Width, this->Height / 2);
	else if (this->Level == 1)
		this->Levels[1].Load("F:\\Repo\\Breakout\\Breakout\\levels\\two.lvl", this->Width, this->Height / 2);
	else if (this->Level == 2)
		this->Levels[2].Load("F:\\Repo\\Breakout\\Breakout\\levels\\three.lvl", this->Width, this->Height / 2);
	else if (this->Level == 3)
		this->Levels[3].Load("F:\\Repo\\Breakout\\Breakout\\levels\\four.lvl", this->Width, this->Height / 2);
}

void Game::ResetPlayer()
{
	// reset player/ball stats
	Player->size = PLAYER_SIZE;
	Player->position = glm::vec2(this->Width / 2.0f - PLAYER_SIZE.x / 2.0f, this->Height - PLAYER_SIZE.y);
	Ball->Reset(Player->position + glm::vec2(PLAYER_SIZE.x / 2.0f - BALL_RADIUS, -(BALL_RADIUS * 2.0f)), INITIAL_BALL_VELOCITY);
}


void Game::ProcessInput(float dt)
{
	if (this->State == GAME_ACTIVE)
	{
		float velocity = PLAYER_VELOCITY * dt;
		if (this->Keys[GLFW_KEY_LEFT])
		{
			if (Player->position.x >= 0.0f)
			{
				Player->position.x -= velocity;
				if (Ball->stuck)
					Ball->position.x -= velocity;
			}
		}
		if (this->Keys[GLFW_KEY_RIGHT])
		{
			if (Player->position.x <= this->Width - Player->size.x)
			{
				Player->position.x += velocity;
				if (Ball->stuck)
					Ball->position.x += velocity;
			}

		}
		if (this->Keys[GLFW_KEY_1])
		{
			this->ResetLevel();
			this->Level = 0;
			this->ResetPlayer();
		}
		if (this->Keys[GLFW_KEY_2])
		{
			this->ResetLevel();
			this->Level = 1;
			this->ResetPlayer();
		}
		if (this->Keys[GLFW_KEY_3])
		{
			this->ResetLevel();
			this->Level = 2;
			this->ResetPlayer();	
		}
		if (this->Keys[GLFW_KEY_4])
		{
			this->ResetLevel();
			this->Level = 3;
			this->ResetPlayer();
		}
		if (this->Keys[GLFW_KEY_SPACE])
			Ball->stuck = false;
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

		// draw particles
		Particles->Draw();

		// draw ball
		Ball->Draw(*Renderer);
	}
}

void Game::DoCollisions()
{
	for (GameObj& box : this->Levels[this->Level].Bricks) {
		if (!box.destroyed)
		{
			Collision collision = CheckCollision(*Ball, box);
			// Get 0 - returns if collision is true or false
			if (std::get<0>(collision))
			{
				if (!box.isSolid)
					box.destroyed = true;
				// Get 1 - returns direction enum
				// Get 2 - returns vector
				Direction dir = std::get<1>(collision);
				glm::vec2 diff_vector = std::get<2>(collision);
				// Check to see if collision horizontal
				if (dir == LEFT || dir == RIGHT)
				{
					Ball->velocity.x = -Ball->velocity.x;
					float penetration = Ball->radius - std::abs(diff_vector.x);
					if (dir == LEFT)
						Ball->position.x += penetration;
					else
						Ball->position.x -= penetration;
				}
				else if (dir == UP || dir == DOWN)
				{
					Ball->velocity.y = -Ball->velocity.y;

					float penetration = Ball->radius - std::abs(diff_vector.y);
					if (dir == UP)
						Ball->position.y -= penetration;
					else
						Ball->position.y += penetration;
				}
				else {
					// Do nothing
				}
			}
		}
	}

	Collision result = CheckCollision(*Ball, *Player);
	if (!Ball->stuck && std::get<0>(result))
	{
		float centerBoard = Player->position.x + Player->size.x / 2.0f;
		float distance = (Ball->position.x + Ball->radius) - centerBoard;
		float percentage = distance / (Player->size.x / 2.0f);

		float strength = 2.0f;
		glm::vec2 oldVelocity = Ball->velocity;
		Ball->velocity.x = INITIAL_BALL_VELOCITY.x * percentage * strength;
		Ball->velocity.y = -1.0f * abs(Ball->velocity.y);
		Ball->velocity = glm::normalize(Ball->velocity) * glm::length(oldVelocity);
	}
}

/* PRIVATE FUNCTIONS */

void Game::loadTexture()
{
	ResourceManager::LoadTexture("F:\\Repo\\Breakout\\Breakout\\Images\\maplestory_background.png", true, "background");
	ResourceManager::LoadTexture("F:\\Repo\\Breakout\\Breakout\\Images\\mushroom.png", true, "face");
	ResourceManager::LoadTexture("F:\\Repo\\Breakout\\Breakout\\Images\\mushroom_Square.png", false, "block");
	ResourceManager::LoadTexture("F:\\Repo\\Breakout\\Breakout\\Images\\slime_solid.png", false, "block_solid");
	ResourceManager::LoadTexture("F:\\Repo\\Breakout\\Breakout\\Images\\paddle.png", true, "paddle");
	ResourceManager::LoadTexture("F:\\Repo\\Breakout\\Breakout\\Images\\particle_orange_circle.png", true, "particle");
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


// Compares target vector to each direction vector in compass array. 
// Closest to in angle is the direction return to the function caller.
// Direction part of enum defined in game.h
Direction VectorDirection(glm::vec2 target)
{
	glm::vec2 compass[] = {
		glm::vec2(0.0f, 1.0f),
		glm::vec2(1.0f, 0.0f),
		glm::vec2(0.0f, -1.0f),
		glm::vec2(-1.0f, 0.0f)
	};

	float max = 0.0f;
	unsigned int best_match = -1;
	for (unsigned int i = 0; i < 4; i++)
	{
		float dot_product = glm::dot(glm::normalize(target), compass[i]);
		if (dot_product > max)
		{
			max = dot_product;
			best_match = i;
		}
	}
	return (Direction)best_match;
}

Collision Game::CheckCollision(BallObject& one, GameObj& two)
{
	glm::vec2 center(one.position + one.radius);
	glm::vec2 aabb_half_extents(two.size.x / 2.0f, two.size.y / 2.0f);
	glm::vec2 aabb_center(
		two.position.x + aabb_half_extents.x,
		two.position.y + aabb_half_extents.y
	);

	glm::vec2 difference = center - aabb_center;
	glm::vec2 clamped = glm::clamp(difference, -aabb_half_extents, aabb_half_extents);

	glm::vec2 closest = aabb_center + clamped;

	difference = closest - center;

	// Returns tuple including collision, direction, and difference vector.
	if (glm::length(difference) <= one.radius)
		return std::make_tuple(true, VectorDirection(difference), difference);
	else
		return std::make_tuple(false, UP, glm::vec2(0.0f, 0.0f));
}


// Clamps a value to a value within given range.
float clamp(float value, float min, float max) {
	return std::max(min, std::min(max, value));
}

