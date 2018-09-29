/**
 * @file Framebuffer.h
 * @brief Utilities for framebuffer objects
 */

#ifndef FRAMEBUFFER_H_
#define FRAMEBUFFER_H_

// Own includes
#include "Entity.h"
#include "Texture.h"

namespace AMG {

/**
 * @class Framebuffer
 * @brief Holds a framebuffer object
 */
class Framebuffer: public Entity {
private:
	Texture **colorTexture;		/**< Holds the color textures */
	Texture *depthTexture;		/**< Holds the depth buffer texture (optional) */
	GLuint depthBuffer;			/**< The depth buffer stored */
	GLuint msDepthBuffer;		/**< Multisampled depth buffer */
	GLuint *colorBuffer;		/**< The color buffers stored */
	int nColorBuffers;			/**< Number of color buffers */
	GLuint fbo;					/**< Frame buffer object ID */
	GLuint msFbo;				/**< Multisampled frame buffer object ID */
	int width;					/**< Framebuffer width, in pixels */
	int height;					/**< Framebuffer height, in pixels */
	void createColorAttachments(int n);
public:
	int getWidth(){ return width; }
	int getHeight(){ return height; }
	GLuint getFbo(){ return fbo; }
	GLuint getMultisampledFbo(){ return msFbo; }
	Texture *getColorTexture(int attachment=0){ return colorTexture[attachment]; }
	Texture *getDepthTexture(){ return depthTexture; }

	Framebuffer();
	Framebuffer(int w, int h, int n=0, int samples=0);
	void createColorTexture(int attachment, GLuint format1=GL_RGB, GLuint format2=GL_RGB, GLuint type=GL_UNSIGNED_BYTE);
	void createDepthTexture();
	void start();
	void end();
	void bind();
	void unbind();
	void blit(Framebuffer *fb, int attachment, GLuint buffers, bool ms=false);
	inline void blitToScreen(){ blit(NULL, 0, GL_COLOR_BUFFER_BIT, true);}
	virtual ~Framebuffer();
};

}

#endif
