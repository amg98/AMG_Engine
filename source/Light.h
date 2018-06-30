/**
 * @file Light.h
 * @brief Description of lighting sources
 */

#ifndef LIGHT_H_
#define LIGHT_H_

// Includes OpenGL
#include <glm/glm.hpp>
using namespace glm;

// Own includes
#include "Shader.h"

namespace AMG {

/**
 * @class Light
 * @brief Holds a lighting source
 */
class Light {
private:
public:
	vec3 position;		/**< Position of the light */
	vec3 color;			/**< Color of the light */

	Light(vec3 position, vec3 color);
	void enable();
	virtual ~Light();
};

}

#endif
