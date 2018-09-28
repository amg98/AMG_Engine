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
	vec3 attenuation;	/**< Attenuation of the light */
	vec3 spotDirection;	/**< Spot direction */
	float spotCutoff;	/**< Spot cutoff */
public:
	vec3 &getPosition(){ return position; }
	vec3 &getColor(){ return color; }
	vec3 &getAttenuation(){ return attenuation; }
	vec3 &getSpotDirection(){ return spotDirection; }
	float &getSpotCutoff(){ return spotCutoff; }

	Light(vec3 position, vec3 color, vec3 attenuation);
	Light(vec3 position, vec3 color) : Light(position, color, vec3(0, 0, 1)) {}
	void setSpotLight(vec3 dir, float cutoff){ spotDirection = dir; spotCutoff = cutoff; }
	void enableDeferred(int id);
	void enable(int id);
	virtual ~Light();
};

}

#endif
