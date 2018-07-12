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
	this->life = 0.0f;
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
	life = elapsedTime / lifeLength;
	return (elapsedTime > lifeLength);
}

/**
 * @brief Operator less than for a Particle, used for sorting
 * @param p Particle to compare this
 * @return Whether this is less than p
 */
bool Particle::operator<(Particle* p){
	vec3 v1 = this->position - Renderer::currentRenderer->camera->position;
	vec3 v2 = p->position - Renderer::currentRenderer->camera->position;
	float d1 = glm::dot(v1, v1);
	float d2 = glm::dot(v2, v2);
	return d1 > d2;
}

/**
 * @brief Destructor for a Particle
 */
Particle::~Particle() {
}

}
