/**
 * @file MotionBlur.h
 * @brief Motion blur effect
 */

#ifndef MOTIONBLUR_H_
#define MOTIONBLUR_H_

// Own includes
#include "Shader.h"
#include "Framebuffer.h"
#include "Sprite.h"

#define AMG_MOTION_BLUR_IMAGES 4

namespace AMG {

class MotionBlur {
private:
	MotionBlur(){}
	static Shader *motionShader;							/**< Motion Blur Shader */
	static Framebuffer *motionFB[AMG_MOTION_BLUR_IMAGES];	/**< Framebuffers for the Motion Blur effect */
	static Framebuffer *outFB;								/**< Output framebuffer */
	static int motionIndex;									/**< Current framebuffer to be used */
public:
	static void initialize(int width, int height);
	static void bind();
	static void unbind();
	static Framebuffer *render();
};

}

#endif
