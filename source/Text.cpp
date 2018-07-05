/**
 * @file Text.cpp
 * @brief Text creation and manipulation
 */

// Own includes
#include "Text.h"
#include "Renderer.h"

namespace AMG {

/**
 * @brief Constructor for a Text
 * @param vertices Vertex buffer
 * @param texcoords Texcoord buffer
 * @param size Size of the buffers
 * @param texture Font texture
 */
Text::Text(float *vertices, float *texcoords, int size, Texture *texture) {
	this->position = vec3(0, 0, 0);
	this->color = vec4(1, 1, 1, 1);
	this->texture = texture;
	addBuffer(vertices, size, 2, GL_FLOAT, true);
	addBuffer(texcoords, size, 2, GL_FLOAT);
}

/**
 * @brief Draws a prepared Text
 */
void Text::draw(){
	texture->enable(0);
	Renderer::currentRenderer->setTransformation(position);
	Renderer::currentRenderer->updateMVP();
	Renderer::currentRenderer->currentShader->setUniform("AMG_SprColor", this->color);
	MeshData::drawRaw();
}

/**
 * @brief Destructor for a Text
 */
Text::~Text() {
}

}
