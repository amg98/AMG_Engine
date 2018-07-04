/** @file Camera.h
  * @brief Camera objects
  * It is under development, just a proof of concept
  */

#ifndef CAMERA_H_
#define CAMERA_H_

// Includes OpenGL
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
using namespace glm;

// Own includes
#include "Entity.h"

namespace AMG {

/**
 * @enum CameraMode
 * @brief Available camera mode
 */
enum CameraMode {
	NO_MOVE_CAMERA = 0,		/**< Static camera */
	FPS_CAMERA,				/**< FPS-ish camera */
};

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
	float rotX;					/**< Rotation in X axis */
	float rotY;					/**< Rotation in Y axis */
	int mode;					/**< Selected mode */
	void computeTrihedron();
public:
	Camera(int mode);
	mat4& getMatrix();
	vec3 getRay();
	void update(GLFWwindow *window, float delta);
	void updateFPS(GLFWwindow *window, float delta);
	virtual ~Camera();
};

}

#endif
