/**
 * @file DeferredRendering.cpp
 * @brief Deferred rendering utility library
 */

// Includes C/C++
#include <random>

// Own includes
#include "DeferredRendering.h"
#include "Renderer.h"

namespace AMG {

// Static data
Framebuffer *DeferredRendering::gBuffer = NULL;
Framebuffer *DeferredRendering::outFB = NULL;
Framebuffer *DeferredRendering::ssaoFB = NULL;
Texture *DeferredRendering::positionTexture = NULL;
Texture *DeferredRendering::normalTexture = NULL;
Texture *DeferredRendering::albedoTexture = NULL;
Texture *DeferredRendering::ssaoTexture = NULL;
Shader *DeferredRendering::renderShader = NULL;
Shader *DeferredRendering::ssaoShader = NULL;
Shader *DeferredRendering::blurShader = NULL;
Sprite *DeferredRendering::renderSprite = NULL;
std::vector<Light*> DeferredRendering::lights;
float *DeferredRendering::ssaoKernel = NULL;
Texture *DeferredRendering::ssaoNoiseTexture = NULL;
Texture *DeferredRendering::ssaoBlurTexture = NULL;

/**
 * @brief Initialize the deferred rendering engine
 * @note Water can't be rendered in this mode
 */
void DeferredRendering::initialize(){

	// Create the G-buffer
	int width = Renderer::getWidth();
	int height = Renderer::getHeight();
	gBuffer = new Framebuffer(width, height, 3, 4);
	outFB = new Framebuffer(width, height, 1, 4);
	ssaoFB = new Framebuffer(width, height, 1, 4);

	// Create the G-buffer textures
	glBindFramebuffer(GL_FRAMEBUFFER, gBuffer->getFbo());
	positionTexture = new Texture(width, height, GL_RGB16F, GL_RGB, GL_COLOR_ATTACHMENT0, GL_FLOAT);
	normalTexture = new Texture(width, height, GL_RGB16F, GL_RGB, GL_COLOR_ATTACHMENT1, GL_FLOAT);
	albedoTexture = new Texture(width, height, GL_RGBA, GL_RGBA, GL_COLOR_ATTACHMENT2, GL_UNSIGNED_BYTE);
	glBindFramebuffer(GL_FRAMEBUFFER, ssaoFB->getFbo());
	ssaoTexture = new Texture(width, height, GL_R16F, GL_RGB, GL_COLOR_ATTACHMENT0, GL_FLOAT);
	glBindFramebuffer(GL_FRAMEBUFFER, outFB->getFbo());
	ssaoBlurTexture = new Texture(width, height, GL_R16F, GL_RGB, GL_COLOR_ATTACHMENT0, GL_FLOAT);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// Load shaders
	renderShader = new Shader("Deferred/AMG_Render.vs", "Deferred/AMG_Render.fs");
	ssaoShader = new Shader("Deferred/AMG_SSAO.vs", "Deferred/AMG_SSAO.fs");
	blurShader = new Shader("Deferred/AMG_SSAOBlur.vs", "Deferred/AMG_SSAOBlur.fs");

	// Create the rendering sprite
	renderSprite = new Sprite();
	renderSprite->getPosition() = vec3(width / 2.0f, height / 2.0f, 0.0f);
	renderSprite->getScaleY() = -1.0f;

	// Initialize the lights vector
	lights = std::vector<Light*>();

	// Generate a kernel for SSAO
	std::uniform_real_distribution<float> randomFloats(0.0, 1.0);
	std::default_random_engine generator;
	ssaoKernel = (float*) malloc (64 * sizeof(float) * 3);
	for (unsigned int i = 0; i < 64; i++){
		int offset = i * 3;
	    vec3 sample(
	        randomFloats(generator) * 2.0 - 1.0,
	        randomFloats(generator) * 2.0 - 1.0,
	        randomFloats(generator)
	    );
	    sample  = glm::normalize(sample);
	    sample *= randomFloats(generator);
	    float scale = (float)i / 64.0;
	    sample *= (0.1f + 0.9f * scale * scale);
	    ssaoKernel[offset + 0] = sample.x;
	    ssaoKernel[offset + 1] = sample.y;
	    ssaoKernel[offset + 2] = sample.z;
	}

	// Generate a noise pattern for SSAO
	float ssaoNoise[16 * 3];
	for (unsigned int i = 0; i < 16; i++){
		int offset = i * 3;
		ssaoNoise[offset + 0] = randomFloats(generator) * 2.0f - 1.0f;
		ssaoNoise[offset + 1] = randomFloats(generator) * 2.0f - 1.0f;
		ssaoNoise[offset + 2] = 0.0f;
	}
	ssaoNoiseTexture = new Texture();
	ssaoNoiseTexture->loadFloatData(4, 4, ssaoNoise);
}

/**
 * @brief Start the deferred rendering pass
 */
void DeferredRendering::start(){
	glDisable(GL_BLEND);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	gBuffer->bind();
}

/**
 * @brief Stop the deferred rendering pass
 */
void DeferredRendering::end(){
	gBuffer->unbind();
	glEnable(GL_BLEND);
}

/**
 * @brief Do the rendering process and return a framebuffer with it
 * @param blitDepth Blit the depth buffer? (default yes)
 */
void DeferredRendering::render(bool blitDepth){

	// Get the current view
	mat4 view = Renderer::getView();
	mat3 view3x3 = mat3(view);
	Renderer::set3dMode(false);

	// Render SSAO image
	glDisable(GL_BLEND);
	ssaoShader->enable();
	ssaoShader->setUniform("AMG_SSAOProjection", Renderer::getPerspective());
	ssaoShader->setUniform3fv("AMG_SSAOSamples", 64, ssaoKernel);
	ssaoFB->bind();
	renderSprite->set(positionTexture);
	normalTexture->bind(1);
	ssaoNoiseTexture->bind(2);
	renderSprite->draw();

	// Blur the SSAO texture
	outFB->bind();
	blurShader->enable();
	renderSprite->set(ssaoTexture);
	renderSprite->draw();
	outFB->unbind();

	// Enable the deferred rendering shader
	renderShader->enable();

	// Update lighting information
	renderShader->setUniform("AMG_NLights", (int)lights.size());
	renderShader->setUniform("AMG_DView", view3x3);
	for(unsigned int i=0;i<lights.size();i++){
		lights[i]->enableDeferred(i);
	}

	// Render the scene
	renderSprite->set(positionTexture);
	normalTexture->bind(1);
	albedoTexture->bind(2);
	ssaoBlurTexture->bind(3);
	renderSprite->draw();

	// Copy the depth buffer, if needed
	if(blitDepth)
		gBuffer->blitDepthBuffer();

	// Restore the rendering settings
	Renderer::set3dMode(true);
	Renderer::setView(view);
	glEnable(GL_BLEND);
}

/**
 * @brief Finish the deferred rendering engine
 */
void DeferredRendering::finish(){
	if(gBuffer) delete gBuffer;
	if(positionTexture) delete positionTexture;
	if(normalTexture) delete normalTexture;
	if(albedoTexture) delete albedoTexture;
	if(renderShader) delete renderShader;
	if(renderSprite) delete renderSprite;
	if(outFB) delete outFB;
	if(ssaoNoiseTexture) delete ssaoNoiseTexture;
	if(ssaoShader) delete ssaoShader;
	if(ssaoFB) delete ssaoFB;
	if(ssaoKernel) free(ssaoKernel);
	if(ssaoTexture) delete ssaoTexture;
	if(blurShader) delete blurShader;
	if(ssaoBlurTexture) delete ssaoBlurTexture;
}

}
