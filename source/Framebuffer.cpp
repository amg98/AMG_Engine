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

	// Check the number of color buffers
	if(n < 1) n = 1;
	nColorBuffers = n;

	// Create the multisampled framebuffer object, if needed
	if(samples > 0){
		glGenFramebuffers(1, &msFbo);
		glBindFramebuffer(GL_FRAMEBUFFER, msFbo);
		glGenRenderbuffers(1, &msDepthBuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, msDepthBuffer);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, w, h);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, msDepthBuffer);
		createColorAttachments(n);
	}else{
		msFbo = 0;
		msDepthBuffer = 0;
	}

	// Create a framebuffer object
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	// Create the color attachment data
	createColorAttachments(n);

	// Create the color texture slots
	colorTexture = (Texture**) calloc (n, sizeof(Texture*));

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

	// Check the multisampled framebuffer
	if(msFbo){
		glBindFramebuffer(GL_FRAMEBUFFER, msFbo);
		if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE){
			Debug::showError(FRAMEBUFFER_CREATION, NULL);
		}
	}

	// Unbind the framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);	// Set to 0 for the default framebuffer creation
}

/**
 * @brief Create the color attachment data
 */
void Framebuffer::createColorAttachments(int n){
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
}

/**
 * @brief Creates a depth texture for this Framebuffer
 */
void Framebuffer::createDepthTexture(){
	GLuint id = (msFbo) ? msFbo : fbo;
	glBindFramebuffer(GL_FRAMEBUFFER, id);
	depthTexture = new Texture(width, height, GL_DEPTH_COMPONENT16, GL_DEPTH_COMPONENT, GL_DEPTH_ATTACHMENT);
	unbind();
}

/**
 * @brief Create a render texture for this Framebuffer (not multisampled)
 * @param attachment Color attachment to use
 */
void Framebuffer::createColorTexture(int attachment, GLuint format1, GLuint format2, GLuint type){
	if(attachment >= nColorBuffers) return;
	GLuint id = (msFbo) ? msFbo : fbo;
	glBindFramebuffer(GL_FRAMEBUFFER, id);
	colorTexture[attachment] = new Texture(width, height, format1, format2, GL_COLOR_ATTACHMENT0 + attachment, type);
	unbind();
}

/**
 * @brief Start rendering with this framebuffer
 */
void Framebuffer::start(){
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glViewport(0, 0, width, height);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

/**
 * @brief Finish rendering with this framebuffer, computing multisampling
 */
void Framebuffer::end(){
	if(msFbo){
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, msFbo);
		glBindFramebuffer(GL_READ_FRAMEBUFFER, fbo);
		for(int i=0;i<nColorBuffers;i++){
			glReadBuffer(GL_COLOR_ATTACHMENT0 + i);
			glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
		}
		glReadBuffer(GL_COLOR_ATTACHMENT0);
		glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
	}

	unbind();
}

/**
 * @brief Bind this Framebuffer to be the current one
 * @param w Width in pixels
 * @param h Height in pixels
 */
void Framebuffer::bind(){
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
}

/**
 * @brief Unbind this Framebuffer
 */
void Framebuffer::unbind(){
	glBindFramebuffer(GL_FRAMEBUFFER, Renderer::get3dFramebuffer()->getFbo());
	glViewport(0, 0, Renderer::getWidth(), Renderer::getHeight());
}

/**
 * @brief Blits a framebuffer
 * @param fb Framebuffer to blit to
 * @param attachment Number of color attachment to use (0-7)
 * @param buffers Buffer copy flags
 * @param ms Blit to the multisampled image? (false by default)
 */
void Framebuffer::blit(Framebuffer *fb, int attachment, GLuint buffers, bool ms){
	Framebuffer *f = (fb == NULL) ? Renderer::get3dFramebuffer() : fb;
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, ms ? f->getMultisampledFbo() : f->getFbo());
	glBindFramebuffer(GL_READ_FRAMEBUFFER, fbo);
	if(fb == NULL) glDrawBuffer(GL_BACK);
	if(buffers &GL_COLOR_BUFFER_BIT) glReadBuffer(GL_COLOR_ATTACHMENT0 + attachment);
	glBlitFramebuffer(0, 0, width, height, 0, 0, f->getWidth(), f->getHeight(), buffers, GL_NEAREST);
	glBindFramebuffer(GL_FRAMEBUFFER, Renderer::get3dFramebuffer()->getFbo());
}

/**
 * @brief Destructor for a RenderTexture
 */
Framebuffer::~Framebuffer() {
	glDeleteFramebuffers(1, &fbo);
	glDeleteRenderbuffers(1, &depthBuffer);
	if(colorTexture && nColorBuffers > 0){
		for(int i=0;i<nColorBuffers;i++){
			if(colorTexture[i]) delete colorTexture[i];
		}
		free(colorTexture);
	}
	if(depthTexture) delete depthTexture;
	if(colorBuffer && nColorBuffers > 0){
		glDeleteRenderbuffers(nColorBuffers, colorBuffer);
		free(colorBuffer);
	}
	if(msFbo) glDeleteFramebuffers(1, &msFbo);
	if(msDepthBuffer) glDeleteRenderbuffers(1, &msDepthBuffer);
}

}
