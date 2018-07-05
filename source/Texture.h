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
#include <glm/glm.hpp>
using namespace glm;

// Own includes
#include "Entity.h"

namespace AMG {

#define AMG_CUBE_SIDES 6		/**< Number of faces a cube has */

/**
 * @class Texture
 * @brief Class defining a Texture object
 */
class Texture : public Entity {
private:
	GLuint id;				/**< Texture OpenGL internal ID */
	int nframes;			/**< Number of frames in the sprite */
	int horizontalFrames;	/**< Number of horizontal frames in the sprite */
	int verticalFrames;		/**< Number of vertical frames in the sprite */
	GLuint target;			/**< Texture mode */
protected:
	int width;						/**< Texture width, in pixels (must be a power of two) */
	int height;						/**< Texture height, in pixels (must be a power of two) */
	vec2 texPosition;				/**< Shader texture position */
	vec2 texScale;					/**< Shader texture scale */
public:
	float currentFrame;				/**< Current frame to show, it will be truncated */
	static void loadTexture(const char *path, GLuint target, int *w, int *h);
	void loadTexture(const char *path, float bias);
	Texture(const char *path, float bias=-0.4f);
	Texture(const char **names);
	Texture(const char *path, int frameWidth, int frameHeight, float bias=-0.4f);
	void enable(int slot);
	int getWidth();
	int getHeight();
	virtual ~Texture();
};

}

#endif
