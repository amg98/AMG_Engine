/**
 * @file Framebuffer.cpp
 * @brief Utilities for framebuffer objects
 */

// Own includes
#include "Renderer.h"
#include "Debug.h"
#include "Framebuffer.h"

namespace AMG {

/**
 * @brief Default constructor for a Framebuffer
 */
Framebuffer::Framebuffer() : Framebuffer(Renderer::getWidth(), Renderer::getHeight()) {

}

/**
 * @brief Constructor for a Framebuffer
 * @param w Width of the texture, in pixels
 * @param h Height of the texture, in pixels
 * @param n Number of multisampled color buffers to use (only if you use multisampling)
 * @param samples Samples used
 */
Framebuffer::Framebuffer(int w, int h, int n, int samples) {

	// Initialize data
	this->width = w;
	this->height = h;
	this->colorTexture = NULL;
	this->depthTexture = NULL;
	this->colorBuffer = NULL;

	// Create a framebuffer object
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	if(n < 2){
		glDrawBuffer(GL_COLOR_ATTACHMENT0);
	}else{
		GLuint *colorAttachments = (GLuint*) malloc (n * sizeof(GLuint));
		for(int i=0;i<n;i++){
			colorAttachments[i] = GL_COLOR_ATTACHMENT0 + i;
		}
		glDrawBuffers(n, colorAttachments);
		free(colorAttachments);
	}

	// Create the depth buffer
	glGenRenderbuffers(1, &depthBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
	if(samples == 0){
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, w, h);
	}else{
		glRenderbufferStorageMultisample(GL_RENDERBUFFER, samples, GL_DEPTH_COMPONENT24, w, h);
	}
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);

	// Create the color buffers
	if(n > 0 && samples > 0){
		colorBuffer = (GLuint*) malloc (n * sizeof(GLuint));
		nColorBuffers = n;
		glGenRenderbuffers(n, colorBuffer);
		for(int i=0;i<n;i++){
			glBindRenderbuffer(GL_RENDERBUFFER, colorBuffer[i]);
			glRenderbufferStorageMultisample(GL_RENDERBUFFER, samples, GL_RGBA8, w, h);
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_RENDERBUFFER, colorBuffer[i]);
		}
	}

	// Check that the framebuffer is valid
	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE){
		Debug::showError(FRAMEBUFFER_CREATION, NULL);
	}

	// Unbind the framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

/**
 * @brief Creates a depth texture for this Framebuffer
 */
void Framebuffer::createDepthTexture(){
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	depthTexture = new Texture(width, height, GL_DEPTH_COMPONENT16, GL_DEPTH_COMPONENT, GL_DEPTH_ATTACHMENT);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

/**
 * @brief Create a render texture for this Framebuffer (not multisampled)
 * @param attachment Color attachment to use
 */
void Framebuffer::createColorTexture(int attachment){
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	colorTexture = new Texture(width, height, GL_RGB, GL_RGB, GL_COLOR_ATTACHMENT0 + attachment);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

/**
 * @brief Bind this Framebuffer to be the current one
 * @param w Width in pixels
 * @param h Height in pixels
 */
void Framebuffer::bind(){
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glViewport(0, 0, width, height);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

/**
 * @brief Unbind this Framebuffer
 */
void Framebuffer::unbind(){
	glBindFramebuffer(GL_FRAMEBUFFER, Renderer::get3dFramebuffer()->getFbo());
	glViewport(0, 0, Renderer::getWidth(), Renderer::getHeight());
}

/**
 * @brief Blits a Framebuffer to another Framebuffer (used for multisampling)
 * @param attachment Which color buffer to blit
 * @param fb Framebuffer to be blitted to
 */
void Framebuffer::blit(int attachment, Framebuffer *fb){
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fb->getFbo());
	glBindFramebuffer(GL_READ_FRAMEBUFFER, fbo);
	glReadBuffer(GL_COLOR_ATTACHMENT0 + attachment);
	glBlitFramebuffer(0, 0, width, height, 0, 0, fb->getWidth(), fb->getHeight(), GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT, GL_NEAREST);
	glBindFramebuffer(GL_FRAMEBUFFER, Renderer::get3dFramebuffer()->getFbo());
}

/**
 * @brief Blits a Framebuffer to the screen (only the depth buffer)
 */
void Framebuffer::blitDepthBuffer(){
	Framebuffer *fb = Renderer::get3dFramebuffer();
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fb->getFbo());
	glBindFramebuffer(GL_READ_FRAMEBUFFER, fbo);
	glBlitFramebuffer(0, 0, width, height, 0, 0, Renderer::getWidth(), Renderer::getHeight(), GL_DEPTH_BUFFER_BIT, GL_NEAREST);
	glBindFramebuffer(GL_FRAMEBUFFER, fb->getFbo());
}

/**
 * @brief Blits a Framebuffer to another Framebuffer (only the depth buffer)
 * @param fb Framebuffer to be blitted to
 */
void Framebuffer::blitDepthBuffer(Framebuffer *fb){
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fb->getFbo());
	glBindFramebuffer(GL_READ_FRAMEBUFFER, fbo);
	glBlitFramebuffer(0, 0, width, height, 0, 0, fb->getWidth(), fb->getHeight(), GL_DEPTH_BUFFER_BIT, GL_NEAREST);
	glBindFramebuffer(GL_FRAMEBUFFER, Renderer::get3dFramebuffer()->getFbo());
}

/**
 * @brief Blits a Framebuffer to the screen
 */
void Framebuffer::blit(){
	GLuint dfb = Renderer::get3dFramebuffer()->getFbo();
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, dfb);
	glBindFramebuffer(GL_READ_FRAMEBUFFER, fbo);
	glDrawBuffer(GL_BACK);
	glBlitFramebuffer(0, 0, width, height, 0, 0, Renderer::getWidth(), Renderer::getHeight(), GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT, GL_NEAREST);
	glBindFramebuffer(GL_FRAMEBUFFER, dfb);
}

/**
 * @brief Destructor for a RenderTexture
 */
Framebuffer::~Framebuffer() {
	glDeleteFramebuffers(1, &fbo);
	glDeleteRenderbuffers(1, &depthBuffer);
	if(colorTexture) delete colorTexture;
	if(depthTexture) delete depthTexture;
	if(colorBuffer && nColorBuffers > 0){
		glDeleteRenderbuffers(nColorBuffers, colorBuffer);
		free(colorBuffer);
	}
}

}
