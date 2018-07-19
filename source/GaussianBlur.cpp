/**
 * @file GaussianBlur.cpp
 * @brief Gaussian Blur post-processing effect
 */

// Own includes
#include "GaussianBlur.h"

namespace AMG {

// Internal data
Shader *GaussianBlur::hblurShader = NULL;
Shader *GaussianBlur::vblurShader = NULL;
Framebuffer *GaussianBlur::hblurFB = NULL;
Framebuffer *GaussianBlur::vblurFB = NULL;
Sprite *GaussianBlur::blurSprite = NULL;
bool GaussianBlur::init = false;

/**
 * @brief Initialize objects for this class
 * @param width Display width, in pixels
 * @param height Display height, in pixels
 */
void GaussianBlur::initialize(int width, int height){
	if(init) return;

	hblurShader = new Shader("Effects/AMG_HBlur.vs", "Effects/AMG_Blur.fs", NULL, 0);
	hblurShader->defineUniform("targetWidth");
	hblurShader->enable();
	hblurShader->setUniform("targetWidth", (float)width);
	vblurShader = new Shader("Effects/AMG_VBlur.vs", "Effects/AMG_Blur.fs", NULL, 0);
	vblurShader->defineUniform("targetHeight");
	vblurShader->enable();
	vblurShader->setUniform("targetHeight", (float)height);
	hblurFB = new Framebuffer(width, height);
	hblurFB->createColorTexture(0);
	vblurFB = new Framebuffer(width, height);
	vblurFB->createColorTexture(0);

	if(blurSprite == NULL){
		blurSprite = new Sprite();
		blurSprite->getScaleY() = -1.0f;
	}

	init = true;
}

/**
 * @brief Render the blurred scene
 * @param fb Framebuffer to take the image
 * @return The final framebuffer
 */
Framebuffer *GaussianBlur::render(Framebuffer *fb){

	blurSprite->getPosition().x = fb->getWidth() / 2.0f;
	blurSprite->getPosition().y = fb->getHeight() / 2.0f;

	hblurFB->bind();
	hblurShader->enable();
	blurSprite->set(fb->getColorTexture());
	blurSprite->draw();

	vblurFB->bind();
	vblurShader->enable();
	blurSprite->set(hblurFB->getColorTexture());
	blurSprite->draw();
	vblurFB->unbind();
	return vblurFB;
}

}
