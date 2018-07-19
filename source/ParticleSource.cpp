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

namespace AMG {

/**
 * @brief Constructor for a Particle Source
 * @param texPath Texture atlas path
 * @param hframes Number of horizontal frames
 * @param vframes Number of vertical frames
 */
ParticleSource::ParticleSource(const char *texPath, int hframes, int vframes) {

	// Setup instanced rendering stuff
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, AMG_MAX_PARTICLES * AMG_PVBO_STRIDE, NULL, GL_STREAM_DRAW);
	addInstancedAttribute(2, 4, 0);			// MVP columns
	addInstancedAttribute(3, 4, 4);
	addInstancedAttribute(4, 4, 8);
	addInstancedAttribute(5, 4, 12);
	addInstancedAttribute(6, 4, 16);		// Texture frame position
	addInstancedAttribute(7, 1, 20);		// Texture blend factor
	vboData = (float*) malloc (AMG_MAX_PARTICLES * AMG_PVBO_STRIDE);

	particles = std::list<Particle*>();
	atlas = NULL;
	atlas = new Texture(texPath, hframes, vframes);
	atlas->setDependency();
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
	std::list<Particle*>::iterator i;
	for(i = particles.begin(); i != particles.end(); i++){
		Particle *p = *i;
		if(p->update()){
			particles.remove(p);
		}
	}
	particles.sort();
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
	glBindBuffer(GL_ARRAY_BUFFER, Renderer::quadVertices);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindBuffer(GL_ARRAY_BUFFER, Renderer::quadTexcoords);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, NULL);

	// Bind texture
	atlas->bind(0);

	// Fill particle's buffer
	std::list<Particle*>::iterator i;
	int offset = 0;
	Renderer *renderer = Renderer::currentRenderer;
	for(i = particles.begin(); i != particles.end(); i++){
		Particle *p = *i;
		atlas->getCurrentFrame() = p->getLife() * atlas->getNFrames();
		atlas->animate();
		renderer->setTransformationBillboard(p->getPosition(), p->getRotation(), p->getScale());
		renderer->updateMVP();
		renderer->storeMVP(vboData, offset * 21);
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
	std::list<Particle*>::iterator i;
	for(i = particles.begin(); i != particles.end(); i++){
		particles.erase(i);
		delete (*i);
	}
	if(atlas) delete atlas;
	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);
	free(vboData);
}

}
