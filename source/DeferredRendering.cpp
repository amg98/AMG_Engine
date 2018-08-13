/**
 * @file DeferredRendering.cpp
 * @brief Deferred rendering utility library
 */

// Own includes
#include "DeferredRendering.h"

namespace AMG {

// Static data
Framebuffer *DeferredRendering::gBuffer = NULL;
Framebuffer *DeferredRendering::outFB = NULL;
Texture *DeferredRendering::positionTexture = NULL;
Texture *DeferredRendering::normalTexture = NULL;
Texture *DeferredRendering::albedoTexture = NULL;
Shader *DeferredRendering::renderShader = NULL;
Sprite *DeferredRendering::renderSprite = NULL;

/**
 * @brief Initialize the deferred rendering engine
 */
void DeferredRendering::initialize(){

	// Create the G-buffer
	int width = Renderer::getWidth();
	int height = Renderer::getHeight();
	gBuffer = new Framebuffer(width, height, 3, 4);
	outFB = new Framebuffer(width, height, 1, 4);

	// Create the G-buffer textures
	glBindFramebuffer(GL_FRAMEBUFFER, gBuffer->getFbo());
	positionTexture = new Texture(width, height, GL_RGB16F, GL_RGB, GL_COLOR_ATTACHMENT0, GL_FLOAT);
	normalTexture = new Texture(width, height, GL_RGB16F, GL_RGB, GL_COLOR_ATTACHMENT1, GL_FLOAT);
	albedoTexture = new Texture(width, height, GL_RGBA, GL_RGBA, GL_COLOR_ATTACHMENT2, GL_UNSIGNED_BYTE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// Load the shader
	renderShader = new Shader("Deferred/AMG_Render.vs", "Deferred/AMG_Render.fs");

	// Create the rendering sprite
	renderSprite = new Sprite();
	renderSprite->getPosition() = vec3(width / 2.0f, height / 2.0f, 0.0f);
	renderSprite->getScaleY() = -1.0f;
}

/**
 * @brief Start the deferred rendering pass
 */
void DeferredRendering::start(){
	glDisable(GL_BLEND);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	gBuffer->bind();
}

/**
 * @brief Stop the deferred rendering pass
 */
void DeferredRendering::end(){
	gBuffer->unbind();
	glEnable(GL_BLEND);
}

/**
 * @brief Do the rendering process and return a framebuffer with it
 * @return The resulting framebuffer, don't delete it
 */
Framebuffer *DeferredRendering::render(){
	renderShader->enable();
	outFB->bind();
	Renderer::set3dMode(false);
	renderSprite->set(positionTexture);
	normalTexture->bind(1);
	albedoTexture->bind(2);
	renderSprite->draw();
	Renderer::set3dMode(true);
	outFB->unbind();
	return outFB;
}

/**
 * @brief Finish the deferred rendering engine
 */
void DeferredRendering::finish(){
	if(gBuffer) delete gBuffer;
	if(positionTexture) delete positionTexture;
	if(normalTexture) delete normalTexture;
	if(albedoTexture) delete albedoTexture;
	if(renderShader) delete renderShader;
	if(renderSprite) delete renderSprite;
	if(outFB) delete outFB;
}

}
