/**
 * @file MotionBlur.cpp
 * @brief Motion blur effect
 */

// Own includes
#include "MotionBlur.h"
#include "Renderer.h"

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

	// Load the shader
	motionShader = new Shader("Effects/AMG_MotionBlur");

	// Create the fraebuffers
	for(int i=0;i<AMG_MOTION_BLUR_IMAGES;i++){
		motionFB[i] = new Framebuffer();
		motionFB[i]->createColorTexture(0, GL_RGB16F, GL_RGB, GL_FLOAT);
	}

	// Create the final framebuffer
	outFB = new Framebuffer();
	outFB->createColorTexture(0, GL_RGB16F, GL_RGB, GL_FLOAT);

	// Create a sprite used to draw
	motionSprite = new Sprite();
	motionSprite->getScaleY() = -1.0f;

	// We start with the first framebuffer
	motionIndex = 0;
}

/**
 * @brief Render the blurred scene
 * @param fb The framebuffer to take images
 * @param shader2d 2D shader
 * @return The final framebuffer
 * @note Don't delete the resulting framebuffer
 */
Framebuffer *MotionBlur::render(Framebuffer *fb, Shader *shader2d){

	// Set the sprite's position
	motionSprite->getPosition().x = motionFB[0]->getWidth() / 2.0f;
	motionSprite->getPosition().y = motionFB[0]->getHeight() / 2.0f;

	// Get a copy of the current image
	motionFB[motionIndex]->start();
	shader2d->enable();
	motionSprite->set(fb->getColorTexture());
	motionSprite->draw();
	motionIndex ++;
	motionIndex %= AMG_MOTION_BLUR_IMAGES;

	// Do the motion blur effect
	outFB->start();
	motionShader->enable();
	for(int i=1;i<AMG_MOTION_BLUR_IMAGES;i++){
		motionFB[i]->getColorTexture()->bind(i);
	}
	motionSprite->set(motionFB[0]->getColorTexture());
	motionSprite->draw();
	outFB->end();

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
