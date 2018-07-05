/**
 * @file ParticleSource.cpp
 * @brief A source for tons of particles
 */

// Own includes
#include "ParticleSource.h"
#include "Renderer.h"

namespace AMG {

/**
 * @brief Constructor for a Particle Source
 * @param texPath Texture atlas path
 * @param hframes Number of horizontal frames
 * @param vframes Number of vertical frames
 */
ParticleSource::ParticleSource(const char *texPath, int hframes, int vframes) {
	particles = std::list<Particle*>();
	atlas = NULL;
	atlas = new Texture(texPath, hframes, vframes);
	atlas->setDependency(true);
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
}

/**
 * @brief Draw all the particles in this source
 */
void ParticleSource::draw(){

	glDepthMask(false);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glBindVertexArray(Renderer::quadID);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, Renderer::quadVertices);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindBuffer(GL_ARRAY_BUFFER, Renderer::quadTexcoords);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, NULL);

	// Render each particle
	std::list<Particle*>::iterator i;
	for(i = particles.begin(); i != particles.end(); i++){
		Particle *p = *i;
		atlas->enable(0);
		Renderer::currentRenderer->setTransformationBillboard(p->position, p->rotation, p->scale);
		Renderer::currentRenderer->updateMVP();
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDepthMask(true);
}

/**
 * @brief Destructor for a Particle source
 */
ParticleSource::~ParticleSource() {
	std::list<Particle*>::iterator i;
	for(i = particles.begin(); i != particles.end(); i++){
		particles.remove(*i);
		delete *i;
	}
	if(atlas) delete atlas;
}

}
