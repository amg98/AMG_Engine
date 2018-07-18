/**
 * @file Keyframe.h
 * @brief File describing animation keyframes
 */

#ifndef KEYFRAME_H_
#define KEYFRAME_H_

// Includes OpenGL
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
using namespace glm;

namespace AMG {

/**
 * @class Keyframe
 * @brief Class holding Keyframe information, used in skeletal animation
 * @note Not an Entity, always depends on an Animation object (may be a lot to handle by the trash collector)
 */
class Keyframe {
private:
	float instant;				/**< This keyframe's frame */
	int nmatrices;				/**< Number of matrices (or bones) to modify */
	vec3 *pos;					/**< Position for each bone */
	quat *rot;					/**< Rotation for each bone */
public:
	float getInstant(){ return instant; }
	vec3 &getPosition(int index){ return pos[index]; }
	quat &getRotation(int index){ return rot[index]; }

	Keyframe(float instant, float *data, int nbones);
	virtual ~Keyframe();
};

}

#endif
