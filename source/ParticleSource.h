/**
 * @file ParticleSource.h
 * @brief A source for tons of particles
 */

#ifndef PARTICLESOURCE_H_
#define PARTICLESOURCE_H_

// Includes C/C++
#include <list>

// Includes OpenGL
#include <GL/glew.h>
#include <GLFW/glfw3.h>

// Own includes
#include "Entity.h"
#include "Particle.h"
#include "Texture.h"

namespace AMG {

/**
 * @class ParticleSource
 * @brief Holds a number of particles and processed them, as well as rendering them
 */
class ParticleSource : private Entity {
private:
	Texture *atlas;						/**< Texture atlas for this source */
	GLuint vbo;							/**< VBO for instanced rendering */
	GLuint vao;							/**< VAO for this source */
	float *vboData;						/**< VBO data to be updated */
	std::vector<Particle> particles;	/**< Vector holding all the particles for this source */
	void addInstancedAttribute(int attribute, int dataSize, int offset);
public:
	std::vector<Particle> &getParticles(){ return particles; }

	ParticleSource(const char *texPath, int hframes, int vframes, int maxparticles);
	void update();
	void draw(GLuint alphaFunc);
	virtual ~ParticleSource();
};

}

#endif
