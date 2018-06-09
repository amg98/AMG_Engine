/*
 * Camera.h
 *
 *  Created on: 2 jun. 2018
 *      Author: Andrés
 */

#ifndef CAMERA_H_
#define CAMERA_H_

#define GLEW_STATIC
#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
using namespace glm;

namespace AMG {

enum CameraMode {
	NO_MOVE_CAMERA = 0,
	FPS_CAMERA,
};

class Camera {
private:
	mat4 camera;
	glm::vec3 position;
	float horizontalAngle;
	float verticalAngle;
	float speed;
	float mouseSpeed;
	int mode;
public:
	Camera(int mode);
	mat4& getMatrix();
	void update(GLFWwindow *window);
	void updateFPS(GLFWwindow *window);
	virtual ~Camera();
};

} /* namespace AMG */

#endif /* CAMERA_H_ */
