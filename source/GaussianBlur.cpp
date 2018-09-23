/**
 * @file GaussianBlur.cpp
 * @brief Gaussian Blur post-processing effect
 */

// Own includes
#include "GaussianBlur.h"
#include "Renderer.h"

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

	// If it was initialized
	if(init) return;

	// Create horizontal blur stuff
	hblurShader = new Shader("Effects/AMG_HBlur.vs", "Effects/AMG_Blur.fs");
	hblurShader->defineUniform("targetWidth");
	hblurShader->enable();
	hblurShader->setUniform("targetWidth", (float)width);

	// Create horizontal blur stuff
	vblurShader = new Shader("Effects/AMG_VBlur.vs", "Effects/AMG_Blur.fs");
	vblurShader->defineUniform("targetHeight");
	vblurShader->enable();
	vblurShader->setUniform("targetHeight", (float)height);

	// Create framebuffers
	hblurFB = new Framebuffer(width, height);
	hblurFB->createColorTexture(0);
	vblurFB = new Framebuffer(width, height);
	vblurFB->createColorTexture(0);

	// Create the blur sprite
	blurSprite = new Sprite();
	blurSprite->getScaleY() = -1.0f;

	// All done
	init = true;
}

/**
 * @brief Render the blurred scene
 * @param fb Framebuffer to take the image
 * @return The final framebuffer
 * @note Don't delete the resulting framebuffer
 */
Framebuffer *GaussianBlur::render(Framebuffer *fb){

	// Set the sprite position
	blurSprite->getPosition().x = fb->getWidth() / 2.0f;
	blurSprite->getPosition().y = fb->getHeight() / 2.0f;

	// Do a horizontal blur
	hblurFB->bind();
	hblurShader->enable();
	blurSprite->set(fb->getColorTexture());
	blurSprite->draw();

	// Do a vertical blur
	vblurFB->bind();
	vblurShader->enable();
	blurSprite->set(hblurFB->getColorTexture());
	blurSprite->draw();
	vblurFB->unbind();

	// Return the resulting framebuffer
	return vblurFB;
}

/**
 * @brief Finish the Gaussian Blur system
 */
void GaussianBlur::finish(){
	if(!init) return;
	AMG_DELETE(hblurShader);
	AMG_DELETE(vblurShader);
	AMG_DELETE(hblurFB);
	AMG_DELETE(vblurFB);
	AMG_DELETE(blurSprite);
	init = false;
}

}
