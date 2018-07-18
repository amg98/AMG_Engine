/**
 * @file Skybox.h
 * @brief Environment mapping cubes utilities
 */

#ifndef SKYBOX_H_
#define SKYBOX_H_

// Includes OpenGL
#include <GL/glew.h>
#include <GLFW/glfw3.h>

// Own includes
#include "Object.h"

namespace AMG {

/**
 * @class Skybox
 * @brief Holds a environment made by a cube map
 */
class Skybox: public Object {
public:
	Skybox(const char *dir);
	void draw();
	virtual ~Skybox();
};

}

#endif
