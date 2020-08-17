#pragma once

#ifndef PARTICLE_GENERATOR_H
#define PARTICLE_GENERATOR_H
#include <vector>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "Shader.h"
#include "Texture.h"
#include "GameObj.h"

struct Particle {
    glm::vec2 Position, Velocity;
    glm::vec4 Color;
    float     Life;

    Particle()
        : Position(0.0f), Velocity(0.0f), Color(1.0f), Life(0.0f) { }
};

class ParticleGenerator
{
public:
    ParticleGenerator(Shader shader, Texture texture, unsigned int amount);

    // Updates all the particles
    void Update(float dt, GameObj& obj, unsigned int newParticles, glm::vec2 offset = glm::vec2(0.0f, 0.0f));
    
    // Renders all the particles
    void Draw();
private:
    // State
    std::vector<Particle> particles;
    unsigned int amount;

    // Render state
    Shader shader;
    Texture texture;
    unsigned int VAO;

    // Buffer and vertex attributes
    void init();

    // Returns first particle index thats unused;
    unsigned int firstUnusedParticle();

    // Respawns particle
    void respawnParticle(Particle& particle, GameObj& object, glm::vec2 offset = glm::vec2(0.0f, 0.0f));

};

#endif