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
float DeferredRendering::ssaoKernelSize = 0;
float DeferredRendering::ssaoKernelRadius = 0;
Texture *DeferredRendering::ssaoNoiseTexture = NULL;

/**
 * @brief Initialize the deferred rendering engine
 * @param ambientOcclusion Compute ambient occlusion?
 * @param kernelSize Kernel size for ambient occlusion
 * @param radius
 * @note Water can't be rendered in this mode, and the deferred scene will not be antialiased
 */
void DeferredRendering::initialize(bool ambientOcclusion, float kernelSize, float radius){

	// Create the G-buffer
	int width = Renderer::getWidth();
	int height = Renderer::getHeight();
	gBuffer = new Framebuffer(width, height, 3);

	// Create the G-buffer textures
	gBuffer->createColorTexture(0, GL_RGB16F, GL_RGB, GL_FLOAT);
	gBuffer->createColorTexture(1, GL_RGB16F, GL_RGB, GL_FLOAT);
	gBuffer->createColorTexture(2, GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE);

	// Load shaders
	renderShader = new Shader("Deferred/AMG_Render");

	// Create the rendering sprite
	renderSprite = new Sprite();
	renderSprite->getPosition() = vec3(width / 2.0f, height / 2.0f, 0.0f);
	renderSprite->getScaleY() = -1.0f;

	// Initialize the lights vector
	lights = std::vector<Light*>();

	// Create the SSAO stuff, if needed
	if(ambientOcclusion){

		ssaoFB = new Framebuffer(width, height);
		ssaoFB->createColorTexture(0, GL_R16F, GL_RGB, GL_FLOAT);
		ssaoShader = new Shader("Deferred/AMG_SSAO");
		blurShader = new Shader("Deferred/AMG_SSAOBlur");
		ssaoKernelSize = kernelSize;
		ssaoKernelRadius = radius;

		outFB = new Framebuffer(width, height);
		outFB->createColorTexture(0, GL_R16F, GL_RGB, GL_FLOAT);

		// Generate a kernel for SSAO
		std::uniform_real_distribution<float> randomFloats(0.0, 1.0);
		std::default_random_engine generator;
		ssaoKernel = (float*) malloc (kernelSize * sizeof(float) * 3);
		for (unsigned int i = 0; i < kernelSize; i++){
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
}

/**
 * @brief Start the deferred rendering pass
 * @note Always draw first the deferred part of your scene, as it requieres an empty color buffer to work
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

	// Process the G-buffer
	gBuffer->end();

	// Get the current view
	mat4 view = Renderer::getView();
	Renderer::set3dMode(false);

	// Process Ambient Occlusion, if needed
	if(ssaoShader){

		// Render SSAO image
		ssaoShader->enable();
		ssaoShader->setUniform(AMG_SSAOKernelSize, ssaoKernelSize);
		ssaoShader->setUniform(AMG_SSAOProjection, Renderer::getPerspective());
		ssaoShader->setUniform3fv(AMG_SSAOSamples, 64, ssaoKernel);
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
	}

	// Enable the deferred rendering shader
	renderShader->enable();

	// Update lighting information
	int nlights = lights.size();
	renderShader->setUniform(AMG_NLights, nlights);
	renderShader->setUniform(AMG_DView, view);
	for(unsigned int i=0;i<lights.size();i++){
		lights[i]->enableDeferred(i);
	}

	// Render the scene
	renderSprite->set(gBuffer->getColorTexture(0));
	gBuffer->getColorTexture(1)->bind(1);
	gBuffer->getColorTexture(2)->bind(2);
	if(outFB) outFB->getColorTexture(0)->bind(3);		// Bind the SSAO texture, if needed
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
