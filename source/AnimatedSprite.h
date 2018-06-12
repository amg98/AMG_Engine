/*
 * AnimatedSprite.h
 *
 *  Created on: 12 jun. 2018
 *      Author: Andrés
 */

#ifndef ANIMATEDSPRITE_H_
#define ANIMATEDSPRITE_H_

#include "Sprite.h"

namespace AMG {

class AnimatedSprite: public Sprite {
private:
	int nframes;
	int horizontalFrames;
	int verticalFrames;
public:
	float currentFrame;

	AnimatedSprite(const char *path, int frameWidth, int frameHeight);
	void draw(Renderer *renderer);
	virtual ~AnimatedSprite();
};

} /* namespace AMG */

#endif /* ANIMATEDSPRITE_H_ */
