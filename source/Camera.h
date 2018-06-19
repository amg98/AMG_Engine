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
	glm::vec3 position;			/**< Camera position */
	float horizontalAngle;		/**< Camera horizontal angle */
	float verticalAngle;		/**< Camera vertical angle */
	float speed;				/**< Camera walking speed */
	float mouseSpeed;			/**< Camera mouse speed */
	int mode;					/**< Selected mode */
public:
	Camera(int mode);
	mat4& getMatrix();
	void update(GLFWwindow *window);
	void updateFPS(GLFWwindow *window);
	virtual ~Camera();
};

}

#endif
