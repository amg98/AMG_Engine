/*
 * AnimatedSprite.cpp
 *
 *  Created on: 12 jun. 2018
 *      Author: Andrés
 */

#include "AnimatedSprite.h"
#include <iostream>

namespace AMG {

AnimatedSprite::AnimatedSprite(const char *path, int frameWidth, int frameHeight) : Sprite(path) {
	this->currentFrame = 0.0f;
	this->texScale.x = (float)frameWidth / (float)width;
	this->texScale.y = (float)frameHeight / (float)height;
	this->horizontalFrames = width / frameWidth;
	this->verticalFrames = height / frameHeight;
	this->nframes = horizontalFrames * verticalFrames;
}

void AnimatedSprite::draw(Renderer *renderer){
	int fr = (int)this->currentFrame;
	if(fr >= this->nframes){
		this->currentFrame = 0.0f;
		fr = 0;
	}

	this->texPosition.x = (fr % horizontalFrames) / (float)horizontalFrames;
	this->texPosition.y = (fr / verticalFrames) / (float)verticalFrames;

	sx *= texScale.x;
	sy *= texScale.y;
	Sprite::draw(renderer);
	sx /= texScale.x;
	sy /= texScale.y;
}

AnimatedSprite::~AnimatedSprite() {

}

} /* namespace AMG */
