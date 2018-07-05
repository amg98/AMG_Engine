/**
 * @file Particle.cpp
 * @brief Particle creation and deletion
 */

// Own includes
#include "Particle.h"
#include "Renderer.h"

namespace AMG {

/**
 * @brief Constructor for a Particle
 * @param position Initial position
 * @param velocity Initial velocity vector
 * @param mass Mass of the particle
 * @param lifeLength How much time is the particle alive?
 * @param rotation Initial rotation
 * @param scale The particle's scale
 */
Particle::Particle(vec3 position, vec3 velocity, float mass, float lifeLength, float rotation, float scale) {
	this->position = position;
	this->velocity = velocity;
	this->mass = mass;
	this->lifeLength = lifeLength;
	this->rotation = rotation;
	this->scale = scale;
	this->elapsedTime = 0.0f;
}

/**
 * @brief Update a particle life cycle
 * @return Whether the particle is dead or not
 */
bool Particle::update(){
	float delta = Renderer::currentRenderer->getDelta();
	velocity.y += -1.0f * mass * delta;
	position += velocity * delta;
	elapsedTime += delta;
	return (elapsedTime > lifeLength);
}

/**
 * @brief Destructor for a Particle
 */
Particle::~Particle() {
}

}
