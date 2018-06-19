/** @file AnimatedSprite.cpp
  * @brief Animated Sprite loading and drawing
  *
  * Frames must be equally distributed in the file (same dimensions per frame,
  * no blank spaces allowed)
  */

#include "AnimatedSprite.h"
#include <iostream>

namespace AMG {

/**
 * @brief Constructor of an animated sprite
 * @param path File to load as an animated sprite
 * @param frameWidth Width of one frame, in pixels
 * @param frameHeight Height of one frame, in pixels
 */
AnimatedSprite::AnimatedSprite(const char *path, int frameWidth, int frameHeight) : Sprite(path) {
	this->currentFrame = 0.0f;
	this->texScale.x = (float)frameWidth / (float)width;
	this->texScale.y = (float)frameHeight / (float)height;
	this->horizontalFrames = width / frameWidth;
	this->verticalFrames = height / frameHeight;
	this->nframes = horizontalFrames * verticalFrames;
}

/**
 * @brief Draws an animated sprite
 * @param renderer Window where to render the sprite
 * @note 2D mode should be set to see the sprite, and a proper shader
 * @note Sprite frame can be floating point, it is truncated here to an index
 */
void AnimatedSprite::draw(Renderer *renderer){

	// Calculate current frame as an integer
	int fr = (int)this->currentFrame;
	if(fr >= this->nframes){
		this->currentFrame = 0.0f;
		fr = 0;
	}

	// Update texture position and scale to select the frame
	this->texPosition.x = (fr % horizontalFrames) / (float)horizontalFrames;
	this->texPosition.y = (fr / verticalFrames) / (float)verticalFrames;

	// Draw the sprite
	sx *= texScale.x;
	sy *= texScale.y;
	Sprite::draw(renderer);
	sx /= texScale.x;
	sy /= texScale.y;
}

/**
 * @brief Destructor of an animated sprite, same as an sprite
 */
AnimatedSprite::~AnimatedSprite() {

}

}
