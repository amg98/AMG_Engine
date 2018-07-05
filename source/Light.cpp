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
 * @param attenuation Light's attenuation
 */
Light::Light(vec3 position, vec3 color, vec3 attenuation) {
	this->position = position;
	this->color = color;
	this->attenuation = attenuation;
}

/**
 * @brief Constructor for a Light
 * @param position Light's position
 * @param color Light's color
 */
Light::Light(vec3 position, vec3 color) : Light(position, color, vec3(0, 0, 1)){

}


/**
 * @brief Update light information onto the shader
 * @param id Light's ID in the shader
 */
void Light::enable(int id){
	Shader *shader = Renderer::currentRenderer->currentShader;
	sprintf(shaderText, "AMG_Light[%d].position", id);
	shader->setUniform(std::string(shaderText), position);
	sprintf(shaderText, "AMG_Lights[%d].color", id);
	shader->setUniform(std::string(shaderText), color);
	sprintf(shaderText, "AMG_Lights[%d].attenuation", id);
	shader->setUniform(std::string(shaderText), attenuation);
}

/**
 * @brief Destructor of the Light
 */
Light::~Light() {
}

}
