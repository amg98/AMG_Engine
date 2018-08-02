/**
 * @file BloomEffect.cpp
 * @brief Makes the objects in the scene look brighter
 */

// Own includes
#include "BloomEffect.h"
#include "GaussianBlur.h"

namespace AMG {

// Internal data
Shader *BloomEffect::brightShader = NULL;
Framebuffer *BloomEffect::brightFB = NULL;
Shader *BloomEffect::combineShader = NULL;
Framebuffer *BloomEffect::combineFB = NULL;
Sprite *BloomEffect::bloomSprite = NULL;

/**
 * @brief Initialize objects for this class
 * @param width Display width, in pixels
 * @param height Display height, in pixels
 * @note Gaussian Blur must be initialized
 */
void BloomEffect::initialize(int width, int height){

	// Load brightness files
	brightShader = new Shader("Effects/AMG_BrightFilter.vs", "Effects/AMG_BrightFilter.fs");
	brightFB = new Framebuffer(width, height);
	brightFB->createColorTexture(0);

	// Load combine files
	combineShader = new Shader("Effects/AMG_CombineEffect.vs", "Effects/AMG_CombineEffect.fs");
	combineFB = new Framebuffer(width, height);
	combineFB->createColorTexture(0);

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
	brightFB->bind();
	brightShader->enable();
	bloomSprite->set(fb->getColorTexture());
	bloomSprite->draw();

	// Do the blurring part
	Framebuffer *blur = GaussianBlur::render(brightFB);

	// Combine the two stages
	combineFB->bind();
	combineShader->enable();
	blur->getColorTexture()->bind(1);
	bloomSprite->set(fb->getColorTexture());
	bloomSprite->draw();
	combineFB->unbind();

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
