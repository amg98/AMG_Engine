/**
 * @file DeferredRendering.h
 * @brief Deferred rendering utility library
 */

#ifndef DEFERREDRENDERING_H_
#define DEFERREDRENDERING_H_

// Includes C/C++
#include <vector>

// Own includes
#include "Framebuffer.h"
#include "Sprite.h"
#include "Light.h"
#include "Shader.h"

namespace AMG {

/**
 * @class DeferredRendering
 * @brief Deferred rendering utility library
 */
class DeferredRendering {
private:
	DeferredRendering(){}
	static Framebuffer *gBuffer;		/**< Scene G-Buffer */
	static Framebuffer *outFB;			/**< Output framebuffer */
	static Framebuffer *ssaoFB;			/**< SSAO framebuffer */
	static Shader *renderShader;		/**< Rendering shader */
	static Shader *ssaoShader;			/**< SSAO shader */
	static Shader *blurShader;			/**< SSAO blur shader */
	static Sprite *renderSprite;		/**< Rendering sprite */
	static float *ssaoKernel;			/**< SSAO kernel */
	static Texture *ssaoNoiseTexture;	/**< SSAO noise texture */
public:
	static std::vector<Light*> lights;		/**< Lights used in the engine */
	static void initialize();
	static void start();
	static void end();
	static void render();
	static void finish();
};

}

#endif
