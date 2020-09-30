#include "Game.h"

#include <iostream>
#include <algorithm>
#include <sstream>
#include <irrKlang/irrKlang.h>
using namespace irrklang;

#include "ResourceManager.h"
#include "Sprite.h"
#include "GameObj.h"
#include "ParticleGenerator.h"
#include "PostProcessing.h"
#include "TextRender.h"




// Game setting inital ball speed
const glm::vec2 INITIAL_BALL_VELOCITY(100.0f, -350.0f);

// Radius of ball object
const float BALL_RADIUS = 12.5f;

// Related Game Class States
Sprite* Renderer;
GameObj* Player;
BallObject* Ball;
ParticleGenerator* Particles;
PostProcessing* Effects;
ISoundEngine* SoundEngine = createIrrKlangDevice();
TextRender* Text;

float ShakeTime = 0.0f;

Game::Game(unsigned int width, unsigned int height)
	: State(GAME_MENU), Keys(), Width(width), Height(height), Lives(3)
{

}

Game::~Game()
{
	delete Renderer;
	delete Player;
	delete Ball;
	delete Particles;
	delete Effects;
	delete Text;
	SoundEngine->drop();
}


void Game::Init()
{
	// Load Shaders
	ResourceManager::LoadShader("sprite.vs", "sprite.fs", nullptr, "sprite");
	ResourceManager::LoadShader("particle.vs", "particle.fs", nullptr, "particle");
	ResourceManager::LoadShader("postprocessing.vs", "postprocessing.fs", nullptr, "postprocessing");

	// Configure Shaders
	glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(this->Width),static_cast<float>(this->Height), 0.0f, -1.0f, 1.0f);

	ResourceManager::GetShader("sprite").Use().SetInteger("image", 0);
	ResourceManager::GetShader("sprite").SetMatrix4("projection", projection);
	ResourceManager::GetShader("particle").Use().SetInteger("sprite", 0);
	ResourceManager::GetShader("particle").SetMatrix4("projection", projection);

	// Load Texture
	loadTexture();


	// Set render specific controls
	Renderer = new Sprite(ResourceManager::GetShader("sprite"));
	Effects = new PostProcessing(ResourceManager::GetShader("postprocessing"), this->Width, this->Height);

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

	// Init sound
	SoundEngine->play2D("E:\\Repo\\Breakout\\Breakout\\music\\bgm.mp3", true);

	// Init Text
	Text = new TextRender(this->Width, this->Height);
	Text->Load("E:\\Repo\\Breakout\\Breakout\\fonts\\cour.ttf",24);

	this->ResetLevel();
	this->ResetPlayer();
}




void Game::Update(float dt)
{
	Ball->Move(dt, this->Width);
	this->DoCollisions();
	Particles->Update(dt, *Ball, 2, glm::vec2(Ball->radius / 2.0f));
	this->UpdatePowerUps(dt);
	if (Ball->position.y >= this->Height)
	{
		--this->Lives;
		if (this->Lives == 0)
		{
			this->ResetLevel();
			this->State = GAME_MENU;
		}
		this->ResetPlayer();
	}
	if (ShakeTime > 0.0f)
	{
		ShakeTime -= dt / 2;
		if (ShakeTime <= 0.0f)
			Effects->Shake = false;
	}

	if (this->State == GAME_ACTIVE && this->Levels[this->Level].isCompleted()) {
		this->ResetLevel();
		this->ResetPlayer();
		this->State = GAME_WIN;
	}
}

void Game::ResetLevel()
{
	if (this->Level == 0)
		this->Levels[0].Load("E:\\Repo\\Breakout\\Breakout\\levels\\one.lvl", this->Width, this->Height / 2);
	else if (this->Level == 1)
		this->Levels[1].Load("E:\\Repo\\Breakout\\Breakout\\levels\\two.lvl", this->Width, this->Height / 2);
	else if (this->Level == 2)
		this->Levels[2].Load("E:\\Repo\\Breakout\\Breakout\\levels\\three.lvl", this->Width, this->Height / 2);
	else if (this->Level == 3)
		this->Levels[3].Load("E:\\Repo\\Breakout\\Breakout\\levels\\four.lvl", this->Width, this->Height / 2);

	this->Lives = 3;
}

void Game::ResetPlayer()
{
	// reset player/ball stats
	Player->size = PLAYER_SIZE;
	Player->position = glm::vec2(this->Width / 2.0f - PLAYER_SIZE.x / 2.0f, this->Height - PLAYER_SIZE.y);
	Ball->Reset(Player->position + glm::vec2(PLAYER_SIZE.x / 2.0f - BALL_RADIUS, -(BALL_RADIUS * 2.0f)), INITIAL_BALL_VELOCITY);

	// also disable all active powerups
	Effects->Chaos = Effects->Confuse = false;
	Ball->transparent = Ball->sticky = false;
	Player->color = glm::vec3(1.0f);
	Ball->color = glm::vec3(1.0f);
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
		//if (this->Keys[GLFW_KEY_1])
		//{
		//	this->ResetLevel();
		//	this->Level = 0;
		//	this->ResetPlayer();
		//}
		//if (this->Keys[GLFW_KEY_2])
		//{
		//	this->ResetLevel();
		//	this->Level = 1;
		//	this->ResetPlayer();
		//}
		//if (this->Keys[GLFW_KEY_3])
		//{
		//	this->ResetLevel();
		//	this->Level = 2;
		//	this->ResetPlayer();	
		//}
		//if (this->Keys[GLFW_KEY_4])
		//{
		//	this->ResetLevel();
		//	this->Level = 3;
		//	this->ResetPlayer();
		//}
		if (this->Keys[GLFW_KEY_SPACE])
			Ball->stuck = false;
	}
	if (this->State == GAME_MENU)
	{
		if (this->Keys[GLFW_KEY_ENTER] && !this->KeysProcessed[GLFW_KEY_ENTER])
		{
			this->State = GAME_ACTIVE;
			this->KeysProcessed[GLFW_KEY_ENTER] = true;
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
	}
	if (this->State == GAME_WIN) 
	{
		if (this->Keys[GLFW_KEY_ENTER])
		{
			this->KeysProcessed[GLFW_KEY_ENTER] = true;
			this->State = GAME_MENU;
		}
	}
}

void Game::Render()
{
	if (this->State == GAME_ACTIVE || this->State == GAME_MENU)
	{
		Effects->BeginRender();

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

		// draw powerups
		for (PowerUp& powerUp : this->PowerUps)
			if (!powerUp.destroyed)
				powerUp.Draw(*Renderer);

		Effects->EndRender();
		Effects->Render(glfwGetTime());

		std::stringstream ss; ss << this->Lives;
		Text->RenderText("Lives:" + ss.str(), 5.0f, 5.0f, 1.0f);

	}
	if (this->State == GAME_MENU)
	{
		Text->RenderText("Please ENTER to start", 250.0f, Height / 1.5, 1.0f);
		Text->RenderText("Press 1 - 4 to select a level", 245.0f, Height / 1.5 + 20.0f, 0.75f);
	}
	if (this->State == GAME_WIN)
	{
		Text->RenderText("CONGRADULATIONS, YOU'VE WON", 250.0f, Height / 1.5, 1.0f);
		Text->RenderText("Press Enter to go return back to the menu.", 245.0f, Height / 1.5 + 20.0f, 0.75f);
	}
}


bool ShouldSpawn(unsigned int chance)
{
	unsigned int random = rand() % chance;
	return random == 0;
}

void Game::SpawnPowerUps(GameObj& block)
{
	if (ShouldSpawn(50)) // 1 in 50 chance
		this->PowerUps.push_back(
			PowerUp("speed", glm::vec3(0.5f, 0.5f, 1.0f), 0.0f, block.position, ResourceManager::GetTexture("powerup_speed")));
	if (ShouldSpawn(50))
		this->PowerUps.push_back(
			PowerUp("sticky", glm::vec3(1.0f, 0.5f, 1.0f), 10.0f, block.position, ResourceManager::GetTexture("powerup_sticky")));
	if (ShouldSpawn(50))
		this->PowerUps.push_back(
			PowerUp("pass-through", glm::vec3(0.5f, 1.0f, 0.5f), 5.0f, block.position, ResourceManager::GetTexture("powerup_passthrough")));
	if (ShouldSpawn(50))
		this->PowerUps.push_back(
			PowerUp("pad-size-increase", glm::vec3(1.0f, 0.6f, 0.4), 0.0f, block.position, ResourceManager::GetTexture("powerup_increase")));
	if (ShouldSpawn(50))
		this->PowerUps.push_back(
			PowerUp("ball-increase", glm::vec3(1.0f, 0.6f, 0.4), 0.0f, block.position, ResourceManager::GetTexture("powerup_multi")));
	if (ShouldSpawn(50))
		this->PowerUps.push_back(
			PowerUp("confuse", glm::vec3(1.0f, 0.3f, 0.3f), 2.0f, block.position, ResourceManager::GetTexture("powerup_confuse")));
	if (ShouldSpawn(50))
		this->PowerUps.push_back(
			PowerUp("chaos", glm::vec3(0.9f, 0.25f, 0.25f), 2.0f, block.position, ResourceManager::GetTexture("powerup_chaos")));
}

void ActivatePowerUp(PowerUp& powerUp)
{
	if (powerUp.Type == "speed")
	{
		Ball->velocity *= 1.2;
	}
	else if (powerUp.Type == "sticky")
	{
		Ball->sticky = true;
		Player->color = glm::vec3(1.0f, 0.5f, 1.0f);
	}
	else if (powerUp.Type == "pass-through")
	{
		Ball->transparent = true;
		Ball->color = glm::vec3(1.0f, 0.5f, 0.5f);
	}
	else if (powerUp.Type == "pad-size-increase")
	{
		Player->size.x += 50;
	}
	else if (powerUp.Type == "ball-increase")
	{

	}
	else if (powerUp.Type == "confuse")
	{
		if (!Effects->Chaos)
			Effects->Confuse = true;
	}
	else if (powerUp.Type == "chaos")
	{
		if (!Effects->Confuse)
			Effects->Chaos = true;
	}
}

bool isOtherPowerUpActive(std::vector<PowerUp>& powerUps, std::string type)
{
	for (const PowerUp& powerUp : powerUps)
	{
		if (powerUp.Activated)
			if (powerUp.Type == type)
				return true;
	}
	return false;
}

void Game::UpdatePowerUps(float dt)
{
	for (PowerUp& powerUp : this->PowerUps)
	{
		powerUp.position += powerUp.velocity * dt;
		if (powerUp.Activated)
		{
			powerUp.Duration -= dt;

			if (powerUp.Duration <= 0.0f)
			{
				// Remove power up from list
				powerUp.Activated = false;
				// Deactivate Effect
				if (powerUp.Type == "sticky") {
					if (!isOtherPowerUpActive(this->PowerUps, "sticky"))
					{
						Ball->sticky = false;
						Player->color = glm::vec3(1.0f);
					}
				} 
				else if (powerUp.Type == "pass-through")
				{
					if (!isOtherPowerUpActive(this->PowerUps, "pass-through"))
					{
						Ball->transparent = false;
						Player->color = glm::vec3(1.0f);
					}
				}
				else if (powerUp.Type == "confuse")
				{
					if (!isOtherPowerUpActive(this->PowerUps, "confuse"))
					{
						Effects->Confuse = false;
					}
				}
				else if (powerUp.Type == "chaos")
				{
					if (!isOtherPowerUpActive(this->PowerUps, "pass-through"))
					{
						Effects->Chaos = false;
					}
				}
			}
		}
	}
	this->PowerUps.erase(std::remove_if(this->PowerUps.begin(), this->PowerUps.end(),
		[](const PowerUp& powerUp) { return powerUp.destroyed && !powerUp.Activated; }
	), this->PowerUps.end());
}

void Game::DoCollisions()
{
	for (GameObj& box : this->Levels[this->Level].Bricks)
	{
		if (!box.destroyed)
		{
			Collision collision = CheckCollision(*Ball, box);
			// Get 0 - returns if collision is true or false
			if (std::get<0>(collision))
			{
				if (!box.isSolid)
				{
					box.destroyed = true;
					this->SpawnPowerUps(box);
					SoundEngine->play2D("E:\\Repo\\Breakout\\Breakout\\music\\beep.wav", false);
				}
				else {
					//Block is solid
					ShakeTime = 0.02f;
					Effects->Shake = true;
					SoundEngine->play2D("E:\\Repo\\Breakout\\Breakout\\music\\solid.wav", false);
				}
				// Get 1 - returns direction enum
				// Get 2 - returns vector
				Direction dir = std::get<1>(collision);
				glm::vec2 diff_vector = std::get<2>(collision);
				// Check to see if collision horizontal
				if (!(Ball->transparent && !box.isSolid)) {
					if (dir == LEFT || dir == RIGHT)
					{
						Ball->velocity.x = -Ball->velocity.x;
						float penetration = Ball->radius - std::abs(diff_vector.x);
						if (dir == LEFT)
							Ball->position.x += penetration;
						else
							Ball->position.x -= penetration;
					}
					else
					{
						Ball->velocity.y = -Ball->velocity.y;

						float penetration = Ball->radius - std::abs(diff_vector.y);
						if (dir == UP)
							Ball->position.y -= penetration;
						else
							Ball->position.y += penetration;
					}
				}
			}
		}
	}
		for (PowerUp& powerUp : this->PowerUps)
		{
			if (!powerUp.destroyed)
			{
				// If the power up reaches the bottom destroy it
				if (powerUp.position.y >= this->Height)
					powerUp.destroyed = true;
				if (CheckCollision(*Player, powerUp))
				{
					ActivatePowerUp(powerUp);
					powerUp.destroyed = true;
					powerUp.Activated = true;
					SoundEngine->play2D("E:\\Repo\\Breakout\\Breakout\\music\\powerup.wav", false);
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

		Ball->stuck = Ball->sticky;

		SoundEngine->play2D("E:\\Repo\\Breakout\\Breakout\\music\\bleep.wav", false);
	}

}

/* PRIVATE FUNCTIONS */

void Game::loadTexture()
{
	ResourceManager::LoadTexture("E:\\Repo\\Breakout\\Breakout\\Images\\maplestory_background1.png", true, "background");
	ResourceManager::LoadTexture("E:\\Repo\\Breakout\\Breakout\\Images\\mushroom.png", true, "face");
	ResourceManager::LoadTexture("E:\\Repo\\Breakout\\Breakout\\Images\\mushroom_Square.png", false, "block");
	ResourceManager::LoadTexture("E:\\Repo\\Breakout\\Breakout\\Images\\slime_solid.png", false, "block_solid");
	ResourceManager::LoadTexture("E:\\Repo\\Breakout\\Breakout\\Images\\paddle.png", true, "paddle");
	ResourceManager::LoadTexture("E:\\Repo\\Breakout\\Breakout\\Images\\particle_orange_circle.png", true, "particle");
	ResourceManager::LoadTexture("E:\\Repo\\Breakout\\Breakout\\Images\\powerup_speed.png", true, "powerup_speed");
	ResourceManager::LoadTexture("E:\\Repo\\Breakout\\Breakout\\Images\\powerup_sticky.png", true, "powerup_sticky");
	ResourceManager::LoadTexture("E:\\Repo\\Breakout\\Breakout\\Images\\powerup_increase.png", true, "powerup_increase");
	ResourceManager::LoadTexture("E:\\Repo\\Breakout\\Breakout\\Images\\powerup_confuse.png", true, "powerup_confuse");
	ResourceManager::LoadTexture("E:\\Repo\\Breakout\\Breakout\\Images\\powerup_chaos.png", true, "powerup_chaos");
	ResourceManager::LoadTexture("E:\\Repo\\Breakout\\Breakout\\Images\\powerup_passthrough.png", true, "powerup_passthrough");
	ResourceManager::LoadTexture("E:\\Repo\\Breakout\\Breakout\\Images\\powerup_multi.png", true, "powerup_multi");
}

void Game::loadLevels()
{
	GameLevel one; 
	GameLevel two; 
	GameLevel three; 
	GameLevel four; 

	one.Load("E:\\Repo\\Breakout\\Breakout\\levels\\one.lvl", this->Width, this->Height / 2);
	two.Load("E:\\Repo\\Breakout\\Breakout\\levels\\two.lvl", this->Width, this->Height / 2);
	three.Load("E:\\Repo\\Breakout\\Breakout\\levels\\three.lvl", this->Width, this->Height / 2);
	four.Load("E:\\Repo\\Breakout\\Breakout\\levels\\four.lvl", this->Width, this->Height / 2);

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

bool Game::CheckCollision(GameObj& one, GameObj& two) // AABB - AABB collision
{
	// collision x-axis?
	bool collisionX = one.position.x + one.size.x >= two.position.x &&
		two.position.x + two.size.x >= one.position.x;
	// collision y-axis?
	bool collisionY = one.position.y + one.size.y >= two.position.y &&
		two.position.y + two.size.y >= one.position.y;
	// collision only if on both axes
	return collisionX && collisionY;
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
