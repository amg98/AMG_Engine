/**
 * @file Texture.h
 * @brief Loading and configuration of textures in general
 */

#ifndef TEXTURE_H_
#define TEXTURE_H_

// Includes OpenGL
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

// Own includes
#include "Entity.h"

namespace AMG {

/**
 * @class Texture
 * @brief Class defining a Texture object
 */
class Texture : public Entity {
private:
	GLuint id;						/**< Texture OpenGL internal ID */
	bool loaded;					/**< Whether the texture has been successfully loaded, just for debugging purposes */
protected:
	int width;						/**< Texture width, in pixels (must be a power of two) */
	int height;						/**< Texture height, in pixels (must be a power of two) */
public:
	Texture(const char *path);
	void enable();
	int getWidth();
	int getHeight();
	virtual ~Texture();
};

}

#endif
