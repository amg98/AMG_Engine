/**
 * @file Framebuffer.h
 * @brief Utilities for framebuffer objects
 */

#ifndef FRAMEBUFFER_H_
#define FRAMEBUFFER_H_

// Own includes
#include "Entity.h"
#include "Texture.h"
#include "Renderer.h"

namespace AMG {

/**
 * @class Framebuffer
 * @brief Holds a framebuffer object
 */
class Framebuffer: public Entity {
private:
	Texture *colorTexture;		/**< Holds the color texture */
	Texture *depthTexture;		/**< Holds the depth buffer texture (optional) */
	GLuint depthBuffer;			/**< The depth buffer stored */
	GLuint *colorBuffer;		/**< The color buffers stored */
	int nColorBuffers;			/**< Number of color buffers */
	GLuint fbo;					/**< Frame buffer object ID */
	int width;					/**< Framebuffer width, in pixels */
	int height;					/**< Framebuffer height, in pixels */
public:
	int getWidth(){ return width; }
	int getHeight(){ return height; }
	GLuint getFbo(){ return fbo; }
	Texture *getColorTexture(){ return colorTexture; }
	Texture *getDepthTexture(){ return depthTexture; }

	Framebuffer() : Framebuffer(Renderer::getWidth(), Renderer::getHeight()) {}
	Framebuffer(int w, int h, int n=0, int samples=0);
	void createColorTexture(int attachment);
	void createDepthTexture();
	void bind();
	void unbind();
	void blit(int attachment, Framebuffer *fb);
	void blit();
	virtual ~Framebuffer();
};

}

#endif
