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

/**
 * @brief Initialize objects for this class
 * @param width Display width, in pixels
 * @param height Display height, in pixels
 */
void BloomEffect::initialize(int width, int height){
	GaussianBlur::initialize(width, height);
	brightShader = new Shader("Effects/AMG_BrightFilter.vs", "Effects/AMG_BrightFilter.fs", NULL, 0);
	brightFB = new Framebuffer(width, height);
	brightFB->createColorTexture(0);
	combineShader = new Shader("Effects/AMG_CombineEffect.vs", "Effects/AMG_CombineEffect.fs", NULL, 0);
	combineFB = new Framebuffer(width, height);
	combineFB->createColorTexture(0);
}

/**
 * @brief Render the bloomed scene
 * @param fb Framebuffer to take the image
 * @return The final framebuffer
 */
Framebuffer *BloomEffect::render(Framebuffer *fb){
	GaussianBlur::blurSprite->getPosition().x = fb->getWidth() / 2.0f;
	GaussianBlur::blurSprite->getPosition().y = fb->getHeight() / 2.0f;

	brightFB->bind();
	brightShader->enable();
	GaussianBlur::blurSprite->set(fb->getColorTexture());
	GaussianBlur::blurSprite->draw();

	Framebuffer *blur = GaussianBlur::render(brightFB);

	combineFB->bind();
	combineShader->enable();
	blur->getColorTexture()->bind(1);
	GaussianBlur::blurSprite->set(fb->getColorTexture());
	GaussianBlur::blurSprite->draw();

	combineFB->unbind();
	return combineFB;
}

}
