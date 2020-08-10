#pragma once

#ifndef SPRITE_H
#define SPRITE_H


#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Texture.h"
#include "Shader.h"


class Sprite
{
public:
	// Constructor ( init shaders / shapes )
	Sprite(Shader& shader);
	~Sprite();

	// Render a defined texture with given sprite
	void DrawSprite(Texture& texture, glm::vec2 position, glm::vec2 size = glm::vec2(10.0f, 10.0f), float rotate = 0.0f, glm::vec3 color = glm::vec3(1.0f));
private:
	// Render state
	Shader shader;
	unsigned int quadVAO;

	// Initalizes and configures the quad's buffer and vertex attributes
	void initRenderData();
};

#endif