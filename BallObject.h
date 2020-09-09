#pragma once

#ifndef BALLOBJECT_H
#define BALLOBJECT_H

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "GameObj.h"
#include "Texture.h"


class BallObject : public GameObj
{
public:
    float radius;
    bool stuck;
    bool sticky, transparent;

    BallObject();
    BallObject(glm::vec2 pos, float radius, glm::vec2 velocity, Texture spirte);

    glm::vec2 Move(float dt, unsigned int window_width);
    void Reset(glm::vec2 position, glm::vec2 velocity);
};

#endif