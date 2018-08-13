/**
 * @file MotionBlur.cpp
 * @brief Motion blur effect
 */

// Own includes
#include "MotionBlur.h"

namespace AMG {

// Internal variables
Shader *MotionBlur::motionShader = NULL;
Framebuffer *MotionBlur::motionFB[AMG_MOTION_BLUR_IMAGES];
Framebuffer *MotionBlur::outFB;
Sprite *MotionBlur::motionSprite;
int MotionBlur::motionIndex;

/**
 * @brief Initialize objects for this class
 */
void MotionBlur::initialize(){

	// Get screen dimensions
	int width = Renderer::getWidth();
	int height = Renderer::getHeight();

	// Load the shader
	motionShader = new Shader("Effects/AMG_MotionBlur.vs", "Effects/AMG_MotionBlur.fs");

	// Create the fraebuffers
	for(int i=0;i<AMG_MOTION_BLUR_IMAGES;i++){
		motionFB[i] = new Framebuffer(width, height);
		motionFB[i]->createColorTexture(0);
	}

	// Create the final framebuffer
	outFB = new Framebuffer(width, height);
	outFB->createColorTexture(0);

	// Create a sprite used to draw
	motionSprite = new Sprite();
	motionSprite->getScaleY() = -1.0f;

	// We start with the first framebuffer
	motionIndex = 0;
}

/**
 * @brief Bind the effect, since then what is drawn is motion blurred
 */
void MotionBlur::bind(){

	// Bind the current framebuffer
	motionFB[motionIndex]->bind();

	// Increase the count
	motionIndex ++;
	motionIndex %= AMG_MOTION_BLUR_IMAGES;
}

/**
 * @brief Stop the effect, rendering is done without blurring
 */
void MotionBlur::unbind(){
	motionFB[0]->unbind();
}

/**
 * @brief Render the blurred scene
 * @return The final framebuffer
 * @note Don't delete the resulting framebuffer
 */
Framebuffer *MotionBlur::render(){

	// Set the sprite's position
	motionSprite->getPosition().x = motionFB[0]->getWidth() / 2.0f;
	motionSprite->getPosition().y = motionFB[0]->getHeight() / 2.0f;

	// Do the motion blur effect
	outFB->bind();
	motionShader->enable();
	for(int i=1;i<AMG_MOTION_BLUR_IMAGES;i++){
		motionFB[i]->getColorTexture()->bind(i);
	}
	motionSprite->set(motionFB[0]->getColorTexture());
	motionSprite->draw();
	outFB->unbind();

	// Return the resulting framebuffer
	return outFB;
}

/**
 * @brief Finish the Motion Blur effect
 */
void MotionBlur::finish(){
	AMG_DELETE(motionShader);
	for(int i=0;i<AMG_MOTION_BLUR_IMAGES;i++){
		AMG_DELETE(motionFB[i]);
	}
	AMG_DELETE(outFB);
	AMG_DELETE(motionSprite);
}

}
