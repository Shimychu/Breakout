#include "ParticleGenerator.h"

ParticleGenerator::ParticleGenerator(Shader shader, Texture texture, unsigned int amount)
	: shader(shader), texture(texture), amount(amount)
{
	this->init();
}

void ParticleGenerator::Update(float dt, GameObj& obj, unsigned int newParticles, glm::vec2 offset)
{
	// Add new particles
	for (unsigned int i = 0; i < newParticles; ++i)
	{
		int unusedParticle = this->firstUnusedParticle();
		this->respawnParticle(this->particles[unusedParticle], object, offset);
	}
	
	// Update all exisiting particles
	for (unsigned int i = 0; i < this->amount; ++i)
	{
		Particle& p = this->particles[i];
		p.Life -= dt; // Reduce life of particle
		// If the life of the particle is still alive
		if (p.Life > 0.0f)
		{
			p.Position -= p.Velocity * dt; // Reduce velocity
			p.Color.a -= dt * 2.5; // Reduce alpha
		}
	}


}

void ParticleGenerator::Draw()
{
}

void ParticleGenerator::init()
{
}

unsigned int lastUsedParticle = 0;
unsigned int ParticleGenerator::firstUnusedParticle()
{
	// Search from last used particle.
	for (unsigned int i = lastUsedParticle; i < this->amount; ++i) {
		if (particles[i].Life <= 0.0f) {
			lastUsedParticle = i;
			return i;
		}
	}
	// Else linear search
	for (unsigned int i = 0; i < lastUsedParticle; ++i) {
		if (particles[i].Life <= 0.0f) {
			lastUsedParticle = i;
			return i;
		}
	}
	// Else nothing else worked, we just use the first particle again
	lastUsedParticle = 0;
	return 0;
}

void ParticleGenerator::respawnParticle(Particle& particle, GameObj& object, glm::vec2 offset)
{
}
