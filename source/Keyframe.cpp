/**
 * @file Keyframe.cpp
 * @brief File describing animation keyframes
 */

// Includes C/C++
#include <stdio.h>
#include <stdlib.h>
#include <string>

// Includes OpenGL
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

// Own includes
#include "Keyframe.h"

namespace AMG {

/**
 * @brief Constructor for a Keyframe
 * @param instant The keyframe's frame in the animation
 * @param data Data holding positions and rotations for each bone
 * @param nbones Number of bones that this keyframe modifies
 */
Keyframe::Keyframe(float instant, float *data, int nbones) {
	this->instant = instant;
	this->nmatrices = nbones;
	this->pos = (vec3*) malloc (this->nmatrices * sizeof(vec3));
	this->rot = (quat*) malloc (this->nmatrices * sizeof(quat));
	for(int i=0;i<this->nmatrices;i++){
		int j = i * 7;
		this->pos[i] = glm::make_vec3(&data[j]);
		this->rot[i] = glm::make_quat(&data[j + 3]);
	}
}

/**
 * @brief Get the Keyframe's frame in the animation
 * @return The Keyframe's frame in the animation
 */
float Keyframe::getInstant(){
	return instant;
}

/**
 * @brief Get the position of one bone
 * @param index Bone index
 * @return This bone's position
 */
vec3 &Keyframe::getPosition(int index){
	return pos[index];
}

/**
 * @brief Get the rotation of one bone
 * @param index Bone index
 * @return This bone's rotation
 */
quat &Keyframe::getRotation(int index){
	return rot[index];
}

/**
 * @brief Destructor for a Keyframe
 */
Keyframe::~Keyframe() {
	free(this->pos);
	free(this->rot);
}

}
