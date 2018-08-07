/**
 * @file Sprite.cpp
 * @brief Creation and rendering of sprites
 * Used as a template to make more complicated sprite types (e.g. animated sprites)
 */

#include "Sprite.h"
#include "Shader.h"

namespace AMG {

/**
 * @brief Default constructor for a Sprite
 */
Sprite::Sprite(){
	initData();
}

/**
 * @brief Constructor for a Sprite from a Texture
 * @param texture Texture to make this Sprite
 */
Sprite::Sprite(Texture *texture) : Texture(texture){
	initData();
}

/**
 * @brief Constructor for a Sprite, extends a Texture
 * @param path Location of the sprite image (*.dds)
 */
Sprite::Sprite(const char *path) : Texture(path){
	initData();
}

/**
 * @brief Initialize Sprite data
 */
void Sprite::initData(){
	this->position = vec3(0.0f, 0.0f, 0.0f);
	this->rotation = 0.0f;
	this->sx = 1.0f;
	this->sy = 1.0f;
	this->color = vec4(1.0f, 1.0f, 1.0f, 1.0f);
	this->billboardScale = 1.0f;
}

/**
 * @brief Constructor for an Sprite, extends a Texture
 * @param path Location of the sprite image (*.dds)
 * @param frameWidth Width of a frame, in pixels
 * @param frameHeight Height of a frame, in pixels
 */
Sprite::Sprite(const char *path, int frameWidth, int frameHeight) : Texture(path, frameWidth, frameHeight) {
	initData();
}

/**
 * @brief Draw a Sprite in the proper window
 */
void Sprite::draw(){
	Renderer::getCurrentShader()->setUniform("AMG_SprColor", color);
	Renderer::setTransformation(position, glm::quat(glm::vec3(0, 0, rotation)), glm::vec3(sx * texScale.x * width, sy * texScale.y * height, 1.0f));
	Renderer::updateMVP();
	animate();		// Animate texture
	bind(0);		// Bind texture
	Renderer::bindQuad(true);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	unbind(0);
}

/**
 * @brief Draw a Sprite in the proper window (billboard mode)
 */
void Sprite::drawBillboard(){
	Renderer::getCurrentShader()->setUniform("AMG_SprColor", color);
	Renderer::setTransformationBillboard(position, rotation, billboardScale);
	Renderer::updateMVP();
	Renderer::bindQuad(true);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	animate();		// Animate texture
	bind(0);		// Bind texture
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}

/**
 * @brief Destructor for a Sprite, the same as for a Texture
 */
Sprite::~Sprite() {
}

}
