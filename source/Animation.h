/**
 * @file Animation.h
 * @brief File describing Animations to skeletons
 */

#ifndef ANIMATION_H_
#define ANIMATION_H_

// Own includes
#include "Entity.h"
#include "Bone.h"
#include "Keyframe.h"

namespace AMG {

/**
 * @class Animation
 * @brief Holds Animation data, which can be applied to a specific Object
 */
class Animation: public Entity {
private:
	unsigned int nkeyframes;			/**< Number of Keyframes in the animation */
	Keyframe **keyframes;				/**< Buffer holding all the keyframes */
	float length;						/**< Length of the animation, in frames */
	float currentTime;					/**< Current animation time, in frames */
public:
	Animation(Keyframe **keyframelist, int nkeyframes);
	void increaseTime(float delta);
	float getKeyframes(Keyframe **first, Keyframe **last);
	void animateBone(Bone *bone, Keyframe *first, Keyframe *last, float progress);
	virtual ~Animation();
};

}

#endif
