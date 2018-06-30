/**
 * @file Light.cpp
 * @brief Description of lighting sources
 */

// Own includes
#include "Light.h"
#include "Renderer.h"

namespace AMG {

/**
 * @brief Constructor for a Light
 * @param position Light's position
 * @param color Light's color
 */
Light::Light(vec3 position, vec3 color) {
	this->position = position;
	this->color = color;
}

/**
 * @brief Update light information onto the shader
 */
void Light::enable(){
	Renderer::shader->setUniform("lightPosition", position);
	Renderer::shader->setUniform("lightColor", color);
}

/**
 * @brief Destructor of the Light
 */
Light::~Light() {

}

}
