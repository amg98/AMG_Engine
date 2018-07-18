/**
 * @file Texture.h
 * @brief Loading and configuration of textures in general
 */

#ifndef TEXTURE_H_
#define TEXTURE_H_

// Includes OpenGL
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
	float progress;			/**< Blending between frames */
	float currentFrame;				/**< Current frame to show, it will be truncated */
	void loadTexture(const char *path);
protected:
	int width;						/**< Texture width, in pixels (must be a power of two) */
	int height;						/**< Texture height, in pixels (must be a power of two) */
	vec4 texPosition;				/**< Shader texture position (for 2 frames) */
	vec2 texScale;					/**< Shader texture scale */
public:
	int getWidth(){ return width; }
	int getHeight(){ return height; }
	int getNFrames(){ return nframes; }
	float &getCurrentFrame(){ return currentFrame; }

	static void loadTexture(const char *path, GLuint target, int *w, int *h);
	Texture();
	Texture(const char *path);
	Texture(const char **names);
	Texture(const char *path, int frameWidth, int frameHeight);
	void setup(int w, int h, GLuint mode, GLuint mode2, GLuint attachment);
	void setLod(float bias);
	void setAniso(float aniso);
	void bind(int slot);
	void animate();
	void storeFrameData(float *data, int offset);
	virtual ~Texture();
};

}

#endif
