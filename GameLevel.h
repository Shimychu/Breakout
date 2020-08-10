#pragma once


#ifndef GAMELEVEL_H
#define GAMELEVEL_H

#include <vector>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "GameObj.h"
#include "Sprite.h"
#include "ResourceManager.h"


class GameLevel
{
public:
	std::vector<GameObj> Bricks;
	GameLevel() { }
	void Load(const char* file, unsigned int levelWidth, unsigned int levelHeight);
	void Draw(Sprite& renderer);
	bool isCompleted();
private:
	void init(std::vector<std::vector<unsigned int>> tileData, unsigned int levelWidth, unsigned int levelHeight);

};

#endif