/**
 * @file Skybox.h
 * @brief Environment mapping cubes utilities
 */

#ifndef SKYBOX_H_
#define SKYBOX_H_

// Includes OpenGL
#define GLEW_STATIC
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
private:
	Shader *shader;		/**< Shader used for Skybox rendering */
public:
	Skybox(const char *dir, Shader *shader);
	void draw();
	virtual ~Skybox();
};

}

#endif
