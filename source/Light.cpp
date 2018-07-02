/**
 * @file Light.cpp
 * @brief Description of lighting sources
 */

// Own includes
#include "Light.h"
#include "Renderer.h"

namespace AMG {

static char shaderText[64];		/**< Internal char buffer for updating uniforms */

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
 * @param id Light's ID in the shader
 */
void Light::enable(int id){
	sprintf(shaderText, "AMG_Light[%d].position", id);
	Renderer::shader->setUniform(std::string(shaderText), position);
	sprintf(shaderText, "AMG_Lights[%d].color", id);
	Renderer::shader->setUniform(std::string(shaderText), color);
}

/**
 * @brief Destructor of the Light
 */
Light::~Light() {

}

}
