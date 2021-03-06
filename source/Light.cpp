/**
 * @file Light.cpp
 * @brief Description of lighting sources
 */

// Includes C/C++
#include <math.h>

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
	this->spotDirection = vec3(0.0f);
	this->spotCutoff = M_PI;
}

/**
 * @brief Update light information onto the shader (deferred rendering)
 * @param id Light's ID in the shader
 */
void Light::enableDeferred(int id){
	Shader *shader = Renderer::getCurrentShader();
	sprintf(shaderText, "AMG_LightDR[%d].position", id);
	shader->setUniform(std::string(shaderText), position);
	sprintf(shaderText, "AMG_LightDR[%d].color", id);
	shader->setUniform(std::string(shaderText), color);
	sprintf(shaderText, "AMG_LightDR[%d].attenuation", id);
	shader->setUniform(std::string(shaderText), attenuation);
	sprintf(shaderText, "AMG_LightDR[%d].spotDirection", id);
	shader->setUniform(std::string(shaderText), spotDirection);
	sprintf(shaderText, "AMG_LightDR[%d].spotCutoff", id);
	shader->setUniform(std::string(shaderText), cosf(spotCutoff));
}

/**
 * @brief Update light information onto the shader
 * @param id Light's ID in the shader
 */
void Light::enable(int id){
	Shader *shader = Renderer::getCurrentShader();
	sprintf(shaderText, "AMG_Light[%d].position", id);
	shader->setUniform(std::string(shaderText), position);
	sprintf(shaderText, "AMG_Lights[%d].color", id);
	shader->setUniform(std::string(shaderText), color);
	sprintf(shaderText, "AMG_Lights[%d].attenuation", id);
	shader->setUniform(std::string(shaderText), attenuation);
	sprintf(shaderText, "AMG_Lights[%d].spotDirection", id);
	shader->setUniform(std::string(shaderText), spotDirection);
	sprintf(shaderText, "AMG_Lights[%d].spotCutoff", id);
	shader->setUniform(std::string(shaderText), cosf(spotCutoff));
}

/**
 * @brief Destructor of the Light
 */
Light::~Light() {
}

}
