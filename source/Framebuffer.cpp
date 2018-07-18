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
 * @brief Constructor for a Framebuffer
 * @param w Width of the texture, in pixels
 * @param h Height of the texture, in pixels
 * @param n Number of color buffers to use
 * @param samples Samples used
 */
Framebuffer::Framebuffer(int w, int h, int n, int samples) {

	// Initialise data
	depthTexture = NULL;
	this->width = w;
	this->height = h;

	// Create the framebuffer object
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	// Create the depth buffer
	glGenRenderbuffers(1, &depthBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
	if(samples == 1){
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
	}else{
		glRenderbufferStorageMultisample(GL_RENDERBUFFER, samples, GL_DEPTH_COMPONENT, width, height);
	}
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);

	// Set up color attachments
	GLuint *colorAttachments = (GLuint*) malloc (n * sizeof(GLuint));
	for(int i=0;i<n;i++){
		colorAttachments[i] = GL_COLOR_ATTACHMENT0 + i;
	}
	glDrawBuffers(n, colorAttachments);
	free(colorAttachments);

	// Create the color buffers
	colorBuffer = (GLuint*) calloc (n, sizeof(GLuint));
	glGenRenderbuffers(n, colorBuffer);
	for(int i=0;i<n;i++){
		glBindRenderbuffer(GL_RENDERBUFFER, colorBuffer[i]);
		if(samples == 1){
			glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA8, width, height);
		}else{
			glRenderbufferStorageMultisample(GL_RENDERBUFFER, samples, GL_RGBA8, width, height);
		}
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_RENDERBUFFER, colorBuffer[i]);
	}

	// Check that everything is OK
	nColorBuffers = n;
	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE){
		Debug::showError(FRAMEBUFFER_CREATION, NULL);
	}

	// Set the default framebuffer
	unbind();
}

/**
 * @brief Creates a depth texture for this Framebuffer
 */
void Framebuffer::createDepthTexture(){
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	depthTexture = new Texture();
	depthTexture->setup(width, height, GL_DEPTH_COMPONENT32, GL_DEPTH_COMPONENT, GL_DEPTH_ATTACHMENT);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

/**
 * @brief Create a render texture for this Framebuffer
 * @param attachment Color attachment to use
 */
void Framebuffer::createRenderTexture(int attachment){
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	setup(width, height, GL_RGB, GL_RGB, GL_COLOR_ATTACHMENT0 + attachment);
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
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, Renderer::currentRenderer->getWidth(), Renderer::currentRenderer->getHeight());
}

/**
 * @brief Blits a Framebuffer to another Framebuffer (used for multisampling)
 * @param attachment Which color buffer to blit
 * @param tex Framebuffer to be blitted to
 */
void Framebuffer::blit(int attachment, Framebuffer *fb){
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fb->getFbo());
	glBindFramebuffer(GL_READ_FRAMEBUFFER, this->fbo);
	glReadBuffer(GL_COLOR_ATTACHMENT0 + attachment);
	glBlitFramebuffer(0, 0, width, height, 0, 0, fb->width, fb->height, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT, GL_NEAREST);
	unbind();
}

/**
 * @brief Blits a Framebuffer to the screen
 */
void Framebuffer::blit(){
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	glBindFramebuffer(GL_READ_FRAMEBUFFER, this->fbo);
	glDrawBuffer(GL_BACK);
	glBlitFramebuffer(0, 0, width, height, 0, 0, Renderer::currentRenderer->getWidth(), Renderer::currentRenderer->getHeight(), GL_COLOR_BUFFER_BIT, GL_NEAREST);
	unbind();
}

/**
 * @brief Destructor for a RenderTexture
 */
Framebuffer::~Framebuffer() {
	glDeleteFramebuffers(1, &fbo);
	if(depthTexture) delete depthTexture;
	if(depthBuffer) glDeleteRenderbuffers(1, &depthBuffer);
	if(colorBuffer){
		glDeleteRenderbuffers(nColorBuffers, colorBuffer);
		free(colorBuffer);
	}
}

}
