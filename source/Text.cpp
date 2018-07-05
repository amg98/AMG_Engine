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
	this->charWidth = 0.5f;
	this->charEdge = 0.1f;
	this->charBorderWidth = 0.7f;
	this->charBorderEdge = 0.1f;
	this->charShadowOffset = vec2(0.0f, 0.0f);
	this->charOutlineColor = vec3(0.0f, 0.0f, 0.0f);
	addBuffer(vertices, size, 2, GL_FLOAT, true);
	addBuffer(texcoords, size, 2, GL_FLOAT);
}

/**
 * @brief Draws a prepared Text
 */
void Text::draw(){
	Shader *shader = Renderer::currentRenderer->currentShader;
	texture->enable(0);
	shader->setUniform("AMG_CharWidth", charWidth);
	shader->setUniform("AMG_CharEdge", charEdge);
	shader->setUniform("AMG_CharBorderWidth", charBorderWidth);
	shader->setUniform("AMG_CharBorderEdge", charBorderEdge);
	shader->setUniform("AMG_CharShadowOffset", charShadowOffset);
	shader->setUniform("AMG_CharOutlineColor", charOutlineColor);
	shader->setUniform("AMG_SprColor", color);
	Renderer::currentRenderer->setTransformation(position);
	Renderer::currentRenderer->updateMVP();
	MeshData::drawRaw();
}

/**
 * @brief Destructor for a Text
 */
Text::~Text() {
}

}
