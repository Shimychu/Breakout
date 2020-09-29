#pragma once

#ifndef TEXT_H
#define TEXT_H

#include <map>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "texture.h"
#include "shader.h"

// Used to hold all state information relevant to character.
struct Character {
	unsigned int TextureID;
	glm::ivec2 Size;
	glm::ivec2 Bearing;
	unsigned int Horizontal_offset;
};


class TextRender
{
public:
	
	std::map<char, Character> Characters;

	Shader TextShader;

	TextRender(unsigned int width, unsigned int height);

	void Load(std::string font, unsigned int fontSize);

	void RenderText(std::string text, float x, float y, float scale, glm::vec3 color = glm::vec3(1.0f));

private:
	unsigned int VAO, VBO;
};

#endif