#pragma once


#ifndef POST_PROCESSOR_H
#define POST_PROCESSOR_H

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "Texture.h"
#include "Sprite.h"
#include "Shader.h"

/**
* Host all post processing effects for the game. 
* Post processing effects include: Shake, Chaos, Confuse.
* Required to call BeginRender before rendering game and end render after the game.
*/

class PostProcessing
{
public:
	// State
	Shader PostProcessingShader;
	Texture Texture;
	unsigned int Width, Height;

	// Post processing effects
	bool Confuse, Chaos, Shake;

	// Constructor
	PostProcessing(Shader shader, unsigned int width, unsigned int height);

	// Prepare the post processing framebuffer operations before rendering game
	void BeginRender();

	// Called after rendering game so it stores all rendered data into texture object
	void EndRender();

	// Render postprocessing texture quad
	void Render(float time);
private:
	
	// Render state
	unsigned int MSFBO, FBO; //Multisampled FBO. FBO used for blitting MS color-buffer to texture.
	unsigned int RBO; // Used for multisampled color buffer
	unsigned int VAO;

	// Initalize quad for rendering post processing texture.i
	void initRenderData();
};

#endif