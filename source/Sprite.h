/**
 * @file Sprite.h
 * @brief Creation and rendering of sprites
 * Used as a template to make more complicated sprite types (e.g. animated sprites)
 */

#ifndef SPRITE_H_
#define SPRITE_H_

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
	void initData();
public:
	float x;								/**< Sprite horizontal position, in pixels */
	float y;								/**< Sprite vertical position, in pixels */
	float rotation;							/**< Sprite rotation angle, in radians */
	float sx;								/**< Sprite horizontal scale */
	float sy;								/**< Sprite vertical scale */
	vec4 color;								/**< Sprite color and transparency */

	Sprite(const char *path);
	Sprite(const char *path, int frameWidth, int frameHeight);
	void draw();
	virtual ~Sprite();
};

}

#endif
