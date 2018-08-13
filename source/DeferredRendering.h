/**
 * @file DeferredRendering.h
 * @brief Deferred rendering utility library
 */

#ifndef DEFERREDRENDERING_H_
#define DEFERREDRENDERING_H_

// Own includes
#include "Framebuffer.h"
#include "Sprite.h"

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
	static Texture *positionTexture;	/**< Position texture */
	static Texture *normalTexture;		/**< Normal texture */
	static Texture *albedoTexture;		/**< Albedo texture */
	static Shader *renderShader;		/**< Rendering shader */
	static Sprite *renderSprite;		/**< Rendering sprite */
public:
	static void initialize();
	static void start();
	static void end();
	static Framebuffer *render();
	static void finish();
};

}

#endif
