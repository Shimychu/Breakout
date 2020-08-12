
#ifndef GAMEOBJ_H
#define GAMEOBJ_H

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "Texture.h"
#include "Sprite.h"


// Container object for holding all state relevant for single game object.
// Each object in the game needs minimal states as described with game object constructor.
class GameObj
{
public:
	glm::vec2 position, size, velocity;
	glm::vec3 color;
	float rotation;
	bool isSolid;
	bool destroyed;

	Texture sprite;

	GameObj();
	GameObj(glm::vec2 pos, glm::vec2 size, Texture sprite, 
		glm::vec3 color = glm::vec3(1.0f), glm::vec2 velocity = glm::vec2(0.0f, 0.0f));

	virtual void Draw(Sprite &renderer);

};

#endif