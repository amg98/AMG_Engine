/**
 * @file Sprite.h
 * @brief Creation and rendering of sprites
 * Used as a template to make more complicated sprite types (e.g. animated sprites)
 */

#ifndef SPRITE_H_
#define SPRITE_H_

// Includes OpenGL
#include <glm/glm.hpp>
using namespace glm;

// Own includes
#include "Texture.h"
#include "Renderer.h"
#include "Entity.h"

namespace AMG {

/**
 * @class Sprite
 * @brief Sprite object definition, entities which
 * 		  can be drawn in 2D. Used like an interface.
 */
class Sprite : public Texture {
private:
	static bool internalBuffersInit;		/**< Whether the primitive buffers are created */
protected:
	vec2 texPosition;						/**< Shader texture position */
	vec2 texScale;							/**< Shader texture scale */
public:
	float x;								/**< Sprite horizontal position, in pixels */
	float y;								/**< Sprite vertical position, in pixels */
	float rotation;							/**< Sprite rotation angle, in radians */
	float sx;								/**< Sprite horizontal scale */
	float sy;								/**< Sprite vertical scale */
	vec4 color;								/**< Sprite color and transparency */

	Sprite(const char *path);
	void draw(Renderer *renderer);
	virtual ~Sprite();
};

}

#endif
