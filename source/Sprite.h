/*
 * Sprite.h
 *
 *  Created on: 2 jun. 2018
 *      Author: Andrés
 */

#ifndef SPRITE_H_
#define SPRITE_H_

#include "Texture.h"
#include "Renderer.h"

#include <glm/glm.hpp>
using namespace glm;

namespace AMG {

class Sprite {
private:
	Texture *texture;
	static bool internalBuffersInit;
public:
	float x, y;
	float rotation;
	float sx, sy;
	vec4 color;

	Sprite(const char *path);
	void draw(Renderer *renderer);
	virtual ~Sprite();
};

} /* namespace AMG */

#endif /* SPRITE_H_ */
