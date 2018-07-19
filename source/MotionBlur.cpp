/**
 * @file MotionBlur.cpp
 * @brief Motion blur effect
 */

// Own includes
#include "MotionBlur.h"
#include "GaussianBlur.h"

namespace AMG {

// Internal variables
Shader *MotionBlur::motionShader = NULL;
Framebuffer *MotionBlur::motionFB[AMG_MOTION_BLUR_IMAGES];
Framebuffer *MotionBlur::outFB;
int MotionBlur::motionIndex;

/**
 * @brief Initialize objects for this class
 * @param width Display width, in pixels
 * @param height Display height, in pixels
 */
void MotionBlur::initialize(int width, int height){
	motionShader = new Shader("Effects/AMG_MotionBlur.vs", "Effects/AMG_MotionBlur.fs", NULL, AMG_USE_TEXTURE(AMG_MOTION_BLUR_IMAGES));
	for(int i=0;i<AMG_MOTION_BLUR_IMAGES;i++){
		motionFB[i] = new Framebuffer(width, height);
		motionFB[i]->createColorTexture(0);
	}
	outFB = new Framebuffer(width, height);
	outFB->createColorTexture(0);

	if(GaussianBlur::blurSprite == NULL){
		GaussianBlur::blurSprite = new Sprite();
		GaussianBlur::blurSprite->getScaleY() = -1.0f;
	}

	motionIndex = 0;
}

void MotionBlur::bind(){
	motionFB[motionIndex]->bind();
	motionIndex ++;
	motionIndex %= AMG_MOTION_BLUR_IMAGES;
}

void MotionBlur::unbind(){
	motionFB[0]->unbind();
}

/**
 * @brief Render the blurred scene
 * @return The final framebuffer
 */
Framebuffer *MotionBlur::render(){

	GaussianBlur::blurSprite->getPosition().x = motionFB[0]->getWidth() / 2.0f;
	GaussianBlur::blurSprite->getPosition().y = motionFB[0]->getHeight() / 2.0f;

	outFB->bind();
	motionShader->enable();
	for(int i=1;i<AMG_MOTION_BLUR_IMAGES;i++){
		motionFB[i]->getColorTexture()->bind(i);
	}
	GaussianBlur::blurSprite->set(motionFB[0]->getColorTexture());
	GaussianBlur::blurSprite->draw();

	outFB->unbind();
	return outFB;
}

}
