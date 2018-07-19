/**
 * @file BloomEffect.h
 * @brief Makes the objects in the scene look brighter
 */

#ifndef BLOOMEFFECT_H_
#define BLOOMEFFECT_H_

// Own includes
#include "GaussianBlur.h"

namespace AMG {

/**
 * @class BloomEffect
 * @brief Makes this effect possible
 */
class BloomEffect : public GaussianBlur {
private:
	BloomEffect(){}
	static Shader *brightShader;		/**< Bright shader */
	static Framebuffer *brightFB;		/**< Bright framebuffer */
	static Shader *combineShader;		/**< Combine shader */
	static Framebuffer *combineFB;		/**< Combine framebuffer */
public:
	static void initialize(int width, int height);
	static Framebuffer *render(Framebuffer *fb);
};

}

#endif
