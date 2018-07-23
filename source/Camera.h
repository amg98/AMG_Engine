/** @file Camera.h
  * @brief Camera objects
  * It is under development, just a proof of concept
  */

#ifndef CAMERA_H_
#define CAMERA_H_

// Includes OpenGL
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
using namespace glm;

// Own includes
#include "Entity.h"

namespace AMG {

/**
 * @class Camera
 * @brief Class that holds a Camera
 */
class Camera : private Entity {
private:
	mat4 camera;				/**< Camera matrix */
	vec3 position;				/**< Camera position */
	quat rotation;				/**< Camera rotation */
	vec3 forward;				/**< Forward vector */
	vec3 right;					/**< Right vector */
	vec3 rot;					/**< Rotation in euler angles */
	void computeTrihedron();
public:
	vec3 &getPosition(){ return position; }
	vec3 &getRotation(){ return rot; }
	mat4 &getMatrix(){ return camera; }

	Camera();
	vec3 getRay();
	void update(GLFWwindow *window, float delta);
	void lookAt(int side);
	virtual ~Camera();
};

}

#endif
