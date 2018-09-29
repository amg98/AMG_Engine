/**
 * @file BloomEffect.cpp
 * @brief Makes the objects in the scene look brighter
 */

// Own includes
#include "BloomEffect.h"
#include "GaussianBlur.h"
#include "Renderer.h"

namespace AMG {

// Internal data
Shader *BloomEffect::brightShader = NULL;
Framebuffer *BloomEffect::brightFB = NULL;
Shader *BloomEffect::combineShader = NULL;
Framebuffer *BloomEffect::combineFB = NULL;
Sprite *BloomEffect::bloomSprite = NULL;

/**
 * @brief Initialize objects for this class
 * @note Gaussian Blur must be initialized
 */
void BloomEffect::initialize(){

	// Initialize Gaussian Blur, if it is not yet
	GaussianBlur::initialize(Renderer::getWidth(), Renderer::getHeight());

	// Load brightness files
	brightShader = new Shader("Effects/AMG_BrightFilter");
	brightFB = new Framebuffer();
	brightFB->createColorTexture(0, GL_RGB16F, GL_RGB, GL_FLOAT);

	// Load combine files
	combineShader = new Shader("Effects/AMG_CombineEffect");
	combineFB = new Framebuffer();
	combineFB->createColorTexture(0, GL_RGB16F, GL_RGB, GL_FLOAT);

	// Create a sprite for rendering steps
	bloomSprite = new Sprite();
	bloomSprite->getScaleY() = -1.0f;
}

/**
 * @brief Render the bloomed scene
 * @param fb Framebuffer to take the image
 * @return The final framebuffer
 * @note Don't delete the resulting framebuffer
 */
Framebuffer *BloomEffect::render(Framebuffer *fb){

	// Set the sprite position
	bloomSprite->getPosition().x = fb->getWidth() / 2.0f;
	bloomSprite->getPosition().y = fb->getHeight() / 2.0f;

	// Do the brightness part
	brightFB->start();
	brightShader->enable();
	bloomSprite->set(fb->getColorTexture());
	bloomSprite->draw();

	// Do the blurring part
	Framebuffer *blur = GaussianBlur::render(brightFB);

	// Combine the two stages
	combineFB->start();
	combineShader->enable();
	blur->getColorTexture()->bind(1);
	bloomSprite->set(fb->getColorTexture());
	bloomSprite->draw();
	combineFB->end();

	// Return the resulting framebuffer
	return combineFB;
}

/**
 * @brief Finish the bloom effect system
 */
void BloomEffect::finish(){
	AMG_DELETE(brightShader);
	AMG_DELETE(brightFB);
	AMG_DELETE(combineShader);
	AMG_DELETE(combineFB);
	AMG_DELETE(bloomSprite);
}

}
