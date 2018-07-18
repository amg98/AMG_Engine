/**
 * @file Animation.cpp
 * @brief File describing Animations to skeletons
 */

// Includes C/C++
#include <stdlib.h>
#include <math.h>

// Includes OpenGL
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/common.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
using namespace glm;

// Own includes
#include "Animation.h"

namespace AMG {

/**
 * @brief Constructor for an Animation
 * @param keyframelist List of keyframes that this animation holds
 * @param nkeyframes Number of keyframes in the list
 */
Animation::Animation(Keyframe **keyframelist, int nkeyframes) {
	this->keyframes = keyframelist;
	this->nkeyframes = nkeyframes;
	this->currentTime = 0.0f;
	length = keyframes[nkeyframes-1]->getInstant();
}

/**
 * @brief Increase the animation time
 * @param delta Number of frames to increase in this call
 */
void Animation::increaseTime(float delta){
	currentTime += delta;
	if(currentTime > length){
		currentTime = modf(currentTime, &length);
	}
}

/**
 * @brief Get, for the current time of the animation, the frames in between
 * @param first First keyframe obtained
 * @param last Last keyframe obtained
 * @return A float in [0, 1] telling the proximity to the last keyframe (1 is last, 0 is first)
 */
float Animation::getKeyframes(Keyframe **first, Keyframe **last){
	*first = keyframes[0];
	*last = keyframes[0];
	for(unsigned int i=1;i<nkeyframes;i++){
		*last = keyframes[i];
		if((*last)->getInstant() > currentTime){
			break;
		}
		*first = *last;
	}

	return (currentTime - (*first)->getInstant())/((*last)->getInstant() - (*first)->getInstant());
}

/**
 * @brief Animate a bone, interpolating between 2 poses
 * @param bone The bone to be animated
 * @param first The first keyframe
 * @param last The last keyframe
 * @param progress The return value of Animation::getKeyframes()
 */
void Animation::animateBone(Bone *bone, Keyframe *first, Keyframe *last, float progress){

	int id = bone->getID();
	vec3 pos = first->getPosition(id) + (last->getPosition(id) - first->getPosition(id)) * progress;
	quat rot = glm::slerp(first->getRotation(id), last->getRotation(id), progress);

	bone->getCurrentBindMatrix() = glm::translate(mat4(1.0f), pos) * glm::toMat4(rot);
	std::vector<Bone*> &children = bone->getChildren();
	for(unsigned int i=0;i<children.size();i++){
		this->animateBone(children[i], first, last, progress);
	}
}

/**
 * @brief Destructor for an Animation
 */
Animation::~Animation() {
	for(unsigned int i=0;i<nkeyframes;i++){
		if(keyframes[i]) delete keyframes[i];
	}
	if(keyframes) free(this->keyframes);
}

}
