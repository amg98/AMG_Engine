/** @file AnimatedSprite.h
  * @brief Animated Sprite loading and drawing
  *
  * Frames must be equally distributed in the file (same dimensions per frame,
  * no blank spaces allowed)
  */

#ifndef ANIMATEDSPRITE_H_
#define ANIMATEDSPRITE_H_

#include "Sprite.h"

namespace AMG {

/**
 * @class AnimatedSprite
 * @brief Class that holds an animated sprite, extending Sprite
 */
class AnimatedSprite: public Sprite {
private:
	int nframes;			/**< Number of frames in the sprite */
	int horizontalFrames;	/**< Number of horizontal frames in the sprite */
	int verticalFrames;		/**< Number of vertical frames in the sprite */
public:
	float currentFrame;		/**< Current frame to show, it will be truncated */

	AnimatedSprite(const char *path, int frameWidth, int frameHeight);
	void draw(Renderer *renderer);
	virtual ~AnimatedSprite();
};

}

#endif
