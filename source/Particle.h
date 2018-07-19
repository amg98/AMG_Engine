/**
 * @file Particle.h
 * @brief Particle creation and deletion
 */

#ifndef PARTICLE_H_
#define PARTICLE_H_

// Includes OpenGL
#include <glm/glm.hpp>
using namespace glm;

namespace AMG {

/**
 * @class Particle
 * @brief Class that holds a Particle
 */
class Particle {
private:
	float elapsedTime;	/**< How much time it passed from this particle's creation */
	vec3 position;		/**< This particle's position */
	vec3 velocity;		/**< This particle's velocity */
	float mass;			/**< How much this particle is affected by gravity */
	float lifeLength;	/**< Time of life of this particle */
	float rotation;		/**< Particle rotation, in radians */
	float scale;		/**< Particle scale, 1.0f is the default size */
	float life;			/**< Life percentage */
public:
	vec3 &getPosition(){ return position; }
	vec3 &getVelocity(){ return velocity; }
	float &getMass(){ return mass; }
	float &getLifeLength(){ return lifeLength; }
	float &getRotation(){ return rotation; }
	float &getScale(){ return scale; }
	float &getLife(){ return life; }

	Particle(vec3 position, vec3 velocity, float mass, float lifeLength, float rotation, float scale);
	bool update();
	bool operator<(Particle &p);
	virtual ~Particle();
};

}

#endif
