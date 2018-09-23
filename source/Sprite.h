/**
 * @file Sprite.h
 * @brief Creation and rendering of sprites
 * Used as a template to make more complicated sprite types (e.g. animated sprites)
 */

#ifndef SPRITE_H_
#define SPRITE_H_

// Own includes
#include "Texture.h"
#include "Entity.h"

namespace AMG {

/**
 * @class Sprite
 * @brief Sprite object definition, entities which
 * 		  can be drawn in 2D. Used like an interface.
 */
class Sprite : public Texture {
private:
	vec3 position;							/**< Sprite position, in pixels */
	float rotation;							/**< Sprite rotation angle, in radians */
	float sx;								/**< Sprite horizontal scale */
	float sy;								/**< Sprite vertical scale */
	float billboardScale;					/**< Scale of the billboard */
	vec4 color;								/**< Sprite color and transparency */
	void initData();
public:
	vec3 &getPosition(){ return position; }
	float &getRotation(){ return rotation; }
	float &getScaleX(){ return sx; }
	float &getScaleY(){ return sy; }
	float &getBillboardScale(){ return billboardScale; }
	vec4 &getColor(){ return color; }

	Sprite();
	Sprite(Texture *texture);
	Sprite(const char *path);
	Sprite(const char *path, int frameWidth, int frameHeight);
	void draw();
	void drawBillboard();
	virtual ~Sprite();
};

}

#endif
