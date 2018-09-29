/**
 * @file ParticleSource.cpp
 * @brief A source for tons of particles
 */

// Includes C/C++
#include <algorithm>

// Own includes
#include "ParticleSource.h"
#include "Renderer.h"
#include "Debug.h"

// Defines
#define AMG_PVBO_STRIDE 21 * sizeof(float)		/**< Stride for the particle source VBO */

namespace AMG {

/**
 * @brief Constructor for a Particle Source
 * @param texPath Texture atlas path
 * @param hframes Number of horizontal frames
 * @param vframes Number of vertical frames
 * @paramm maxparticles Maximum number of particles
 */
ParticleSource::ParticleSource(const char *texPath, int hframes, int vframes, int maxparticles) {

	// Setup instanced rendering stuff
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, maxparticles * AMG_PVBO_STRIDE, NULL, GL_STREAM_DRAW);
	addInstancedAttribute(2, 4, 0);			// MVP columns
	addInstancedAttribute(3, 4, 4);
	addInstancedAttribute(4, 4, 8);
	addInstancedAttribute(5, 4, 12);
	addInstancedAttribute(6, 4, 16);		// Texture frame position
	addInstancedAttribute(7, 1, 20);		// Texture blend factor
	vboData = (float*) malloc (maxparticles * AMG_PVBO_STRIDE);

	particles = std::vector<Particle>();
	particles.reserve(maxparticles);

	atlas = NULL;
	atlas = new Texture(texPath, hframes, vframes, true);
}

/**
 * @brief Add an instanced attribute to the bind VAO
 */
void ParticleSource::addInstancedAttribute(int attribute, int dataSize, int offset){
	glVertexAttribPointer(attribute, dataSize, GL_FLOAT, GL_FALSE, AMG_PVBO_STRIDE, (void*)(offset * sizeof(float)));
	glVertexAttribDivisor(attribute, 1);
}

/**
 * @brief Update all the particles in this source
 */
void ParticleSource::update(){
	for(unsigned int i=0;i<particles.size();i++){
		Particle &p = particles[i];
		if(p.update()){
			particles.erase(particles.begin() + i);
		}
	}
	std::sort(particles.begin(), particles.end());
}

/**
 * @brief Draw all the particles in this source
 * @param Blending function to apply
 */
void ParticleSource::draw(GLuint alphaFunc){

	// Set blending
	glDepthMask(false);
	glBlendFunc(GL_SRC_ALPHA, alphaFunc);

	// Bind buffers
	glBindVertexArray(vao);
	for(int i=0;i<8;i++){
		glEnableVertexAttribArray(i);
	}
	Renderer::bindQuad(false);

	// Bind texture
	atlas->bind(0);

	// Fill particle's buffer
	int offset = 0;
	for(unsigned int i=0;i<particles.size();i++){
		Particle &p = particles[i];
		atlas->getCurrentFrame() = p.getLife() * atlas->getNFrames();
		atlas->animate();
		Renderer::setTransformationBillboard(p.getPosition(), p.getRotation(), p.getScale());
		Renderer::updateMVP();
		Renderer::storeMVP(vboData, offset * 21);
		atlas->storeFrameData(vboData, offset * 21 + 16);
		offset ++;
	}

	// Update particle's buffer VBO
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, particles.size() * AMG_PVBO_STRIDE, NULL, GL_STREAM_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, particles.size() * AMG_PVBO_STRIDE, vboData);

	// Draw all particles
	glDrawArraysInstanced(GL_TRIANGLES, 0, 6, particles.size());

	// Restore blend function
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDepthMask(true);
	for(int i=0;i<8;i++){
		glDisableVertexAttribArray(i);
	}
}

/**
 * @brief Destructor for a Particle source
 */
ParticleSource::~ParticleSource() {
	AMG_DELETE(atlas);
	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);
	if(vboData) free(vboData);
}

}
