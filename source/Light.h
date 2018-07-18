/**
 * @file Light.h
 * @brief Description of lighting sources
 */

#ifndef LIGHT_H_
#define LIGHT_H_

// Includes OpenGL
#include <glm/glm.hpp>
using namespace glm;

namespace AMG {

/**
 * @class Light
 * @brief Holds a lighting source
 */
class Light {
private:
	vec3 position;		/**< Position of the light */
	vec3 color;			/**< Color of the light */
	vec3 attenuation;	/**< Attenuation of the light, following a parabola */
public:
	vec3 &getPosition(){ return position; }
	vec3 &getColor(){ return color; }
	vec3 &getAttenuation(){ return attenuation; }

	Light(vec3 position, vec3 color, vec3 attenuation);
	Light(vec3 position, vec3 color);
	void enable(int id);
	virtual ~Light();
};

}

#endif
