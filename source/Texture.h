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
	bool loaded;			/**< If the texture is loaded (for debugging purposes) */
protected:
	int width;						/**< Texture width, in pixels (must be a power of two) */
	int height;						/**< Texture height, in pixels (must be a power of two) */
	vec2 texPosition;				/**< Shader texture position */
	vec2 texScale;					/**< Shader texture scale */
public:
	float currentFrame;				/**< Current frame to show, it will be truncated */
	Texture(const char *path);
	Texture(const char *path, int frameWidth, int frameHeight);
	void loadTexture(const char *path);
	void enable(int slot);
	int getWidth();
	int getHeight();
	virtual ~Texture();
};

}

#endif
