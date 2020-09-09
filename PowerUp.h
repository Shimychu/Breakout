#pragma once

#ifndef POWER_UP_H
#define POWER_UP_H

#include "GameObj.h"


// The size of a PowerUp block
const glm::vec2 POWERUP_SIZE(60.0f, 20.0f);
// Velocity a PowerUp block has when spawned
const glm::vec2 VELOCITY(0.0f, 150.0f);



class PowerUp :
    public GameObj
{
public:
    std::string Type;
    float Duration;
    bool Activated;

    // Constructor
    PowerUp(std::string type, glm::vec3 color, float duration, glm::vec2 position, Texture texture)
        : GameObj(position, POWERUP_SIZE, texture, color, VELOCITY), Type(type), Duration(duration), Activated() { } 
};

#endif