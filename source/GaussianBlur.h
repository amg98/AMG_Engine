/**
 * @file GaussianBlur.h
 * @brief Gaussian Blur post-processing effect
 */

#ifndef GAUSSIANBLUR_H_
#define GAUSSIANBLUR_H_

// Own includes
#include "Shader.h"
#include "Framebuffer.h"
#include "Sprite.h"

namespace AMG {

/**
 * @class GaussianBlur
 * @brief Static class with utility functions to blur images
 */
class GaussianBlur {
private:
	static Shader *hblurShader;			/**< Horizontal blur shader */
	static Shader *vblurShader;			/**< Vertical blur shader */
	static Framebuffer *hblurFB;		/**< Horizontal blur framebuffer */
	static Framebuffer *vblurFB;		/**< Vertical blur framebuffer */
	static bool init;					/**< Whether the class is initialized */
protected:
	GaussianBlur(){}
	static Sprite *blurSprite;			/**< General purpose sprite */
public:
	static void initialize(int width, int height);
	static Framebuffer *render(Framebuffer *fb);
};

}

#endif
