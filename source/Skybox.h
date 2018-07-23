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
	Texture *getCubeMap(){ return materials[0]->getTexture(0); }
	Skybox(const char *dir);
	Skybox(Texture *cubeMap);
	void draw();
	virtual ~Skybox();
};

}

#endif
