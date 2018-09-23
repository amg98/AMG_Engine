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
Shader *DeferredRendering::renderShader = NULL;
Shader *DeferredRendering::ssaoShader = NULL;
Shader *DeferredRendering::blurShader = NULL;
Sprite *DeferredRendering::renderSprite = NULL;
std::vector<Light*> DeferredRendering::lights;
float *DeferredRendering::ssaoKernel = NULL;
Texture *DeferredRendering::ssaoNoiseTexture = NULL;

/**
 * @brief Initialize the deferred rendering engine
 * @note Water can't be rendered in this mode, and the deferred scene will not be antialiased
 */
void DeferredRendering::initialize(){

	// Create the G-buffer
	int width = Renderer::getWidth();
	int height = Renderer::getHeight();
	gBuffer = new Framebuffer(width, height, 3);
	outFB = new Framebuffer(width, height);
	ssaoFB = new Framebuffer(width, height);

	// Create the G-buffer textures
	gBuffer->createColorTexture(0, GL_RGB16F, GL_RGB, GL_FLOAT);
	gBuffer->createColorTexture(1, GL_RGB16F, GL_RGB, GL_FLOAT);
	gBuffer->createColorTexture(2, GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE);
	ssaoFB->createColorTexture(0, GL_R16F, GL_RGB, GL_FLOAT);
	outFB->createColorTexture(0, GL_R16F, GL_RGB, GL_FLOAT);

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
	        randomFloats(generator) * 2.0f - 1.0f,
	        randomFloats(generator) * 2.0f - 1.0f,
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
	gBuffer->start();
}

/**
 * @brief Stop the deferred rendering pass
 */
void DeferredRendering::end(){
	gBuffer->end();
	glEnable(GL_BLEND);
}

/**
 * @brief Do the rendering process and return a framebuffer with it
 */
void DeferredRendering::render(){

	// Get the current view
	mat4 view = Renderer::getView();
	mat3 view3x3 = mat3(view);
	Renderer::set3dMode(false);

	// Render SSAO image
	glDisable(GL_BLEND);
	ssaoShader->enable();
	ssaoShader->setUniform("AMG_SSAOProjection", Renderer::getPerspective());
	ssaoShader->setUniform3fv("AMG_SSAOSamples", 64, ssaoKernel);
	ssaoFB->start();
	renderSprite->set(gBuffer->getColorTexture(0));
	gBuffer->getColorTexture(1)->bind(1);
	ssaoNoiseTexture->bind(2);
	renderSprite->draw();
	ssaoFB->end();

	// Blur the SSAO texture
	outFB->start();
	blurShader->enable();
	renderSprite->set(ssaoFB->getColorTexture(0));
	renderSprite->draw();
	outFB->end();

	// Enable the deferred rendering shader
	renderShader->enable();

	// Update lighting information
	renderShader->setUniform("AMG_NLights", (int)lights.size());
	renderShader->setUniform("AMG_DView", view3x3);
	for(unsigned int i=0;i<lights.size();i++){
		lights[i]->enableDeferred(i);
	}

	// Render the scene
	renderSprite->set(gBuffer->getColorTexture(0));
	gBuffer->getColorTexture(1)->bind(1);
	gBuffer->getColorTexture(2)->bind(2);
	outFB->getColorTexture(0)->bind(3);
	renderSprite->draw();

	// Copy the depth buffer
	gBuffer->blit(NULL, 0, GL_DEPTH_BUFFER_BIT);

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
	if(renderShader) delete renderShader;
	if(renderSprite) delete renderSprite;
	if(outFB) delete outFB;
	if(ssaoNoiseTexture) delete ssaoNoiseTexture;
	if(ssaoShader) delete ssaoShader;
	if(ssaoFB) delete ssaoFB;
	if(ssaoKernel) free(ssaoKernel);
	if(blurShader) delete blurShader;
}

}
