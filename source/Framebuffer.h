/**
 * @file Framebuffer.h
 * @brief Utilities for framebuffer objects
 */

#ifndef FRAMEBUFFER_H_
#define FRAMEBUFFER_H_

// Own includes
#include "Sprite.h"

namespace AMG {

/**
 * @class Framebuffer
 * @brief Holds a framebuffer and ways to manipulate it
 */
class Framebuffer: public Sprite {
private:
	Texture *depthTexture;		/**< Holds the depth buffer texture (optional) */
	GLuint depthBuffer;			/**< The depth buffer stored */
	GLuint *colorBuffer;		/**< The color buffers stored */
	int nColorBuffers;			/**< Number of color buffers */
	GLuint fbo;					/**< Frame buffer object ID */
public:
	GLuint getFbo(){ return fbo; }

	Framebuffer(int w, int h, int n=1, int samples=1);
	void createRenderTexture(int attachment);
	void createDepthTexture();
	void bind();
	void unbind();
	void blit(int attachment, Framebuffer *fb);
	void blit();
	virtual ~Framebuffer();
};

}

#endif
