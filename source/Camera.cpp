/** @file Camera.cpp
  * @brief Camera objects
  * It is under development, just a proof of concept
  */

// Includes OpenGL
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/transform.hpp>

// Own includes
#include "Camera.h"
#include "Renderer.h"

namespace AMG {

/**
 * @brief Constructor of a camera
 * @param mode Desired camera mode (under development)
 */
Camera::Camera(int mode) {
	this->position = glm::vec3(0, 0.1f, 5);
	this->camera = glm::lookAt(position, position + glm::vec3(0, 0, -1), glm::vec3(0, 1, 0));
	this->horizontalAngle = 3.14f;
	this->verticalAngle = 0.0f;
	this->speed = 3.0f;
	this->mouseSpeed = 0.005f;
	this->mode = mode;
}

/**
 * @brief Destructor of a camera
 */
Camera::~Camera() {

}

/**
 * @brief Update this camera according to a window's input
 * @param window Window to be read its input
 * @note Called internally by a renderer
 */
void Camera::update(GLFWwindow *window){
	switch(this->mode){
		case FPS_CAMERA:
			this->updateFPS(window);
			break;
		default:
			break;
	}
}

/**
 * @brief Update a FPS-ish camera
 * @param window Window to be read its input
 * @note Called internally by a renderer
 */
void Camera::updateFPS(GLFWwindow *window){

	static double lastTime = glfwGetTime();
	double currentTime = glfwGetTime();
	float deltaTime = float(currentTime - lastTime);

	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);

	Renderer *renderer = (Renderer*)glfwGetWindowUserPointer(window);
	glfwSetCursorPos(window, renderer->width/2, renderer->height/2);

	horizontalAngle += mouseSpeed * deltaTime * float(renderer->width/2 - xpos );
	verticalAngle   += mouseSpeed * deltaTime * float(renderer->height/2 - ypos );

	glm::vec3 direction(
	    cos(verticalAngle) * sin(horizontalAngle),
	    sin(verticalAngle),
	    cos(verticalAngle) * cos(horizontalAngle)
	);

	glm::vec3 right = glm::vec3(
	    sin(horizontalAngle - 3.14f/2.0f),
	    0,
	    cos(horizontalAngle - 3.14f/2.0f)
	);

	glm::vec3 up = glm::cross( right, direction );

	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS){
	    position += direction * deltaTime * speed;
	}

	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS){
	    position -= direction * deltaTime * speed;
	}

	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS){
	    position += right * deltaTime * speed;
	}

	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS){
	    position -= right * deltaTime * speed;
	}

	this->camera = glm::lookAt(position, position+direction, up);

	lastTime = currentTime;
}

/**
 * @brief Get camera matrix
 * @return Camera matrix
 */
mat4& Camera::getMatrix(){
	return camera;
}

}
