/**
 * @file ParticleSource.h
 * @brief A source for tons of particles
 */

#ifndef PARTICLESOURCE_H_
#define PARTICLESOURCE_H_

// Includes C/C++
#include <list>

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
public:
	std::list<Particle*> particles;		/**< Vector holding all the particles for this source */
	ParticleSource(const char *texPath, int hframes, int vframes);
	void update();
	void draw(GLuint alphaFunc);
	virtual ~ParticleSource();
};

}

#endif
