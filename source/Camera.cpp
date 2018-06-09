/*
 * Camera.cpp
 *
 *  Created on: 2 jun. 2018
 *      Author: Andrés
 */

#include "Camera.h"
#include "Renderer.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/transform.hpp>

namespace AMG {

Camera::Camera(int mode) {
	this->position = glm::vec3(0, 0, 5);
	this->camera = glm::lookAt(position, position+glm::vec3(0, 0, -1), glm::vec3(0, 1, 0));
	this->horizontalAngle = 3.14f;
	this->verticalAngle = 0.0f;
	this->speed = 3.0f;
	this->mouseSpeed = 0.005f;
	this->mode = mode;
}

Camera::~Camera() {
	// TODO Auto-generated destructor stub
}

void Camera::update(GLFWwindow *window){
	switch(this->mode){
		case FPS_CAMERA:
			this->updateFPS(window);
			break;
		default:
			break;
	}
}

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

mat4& Camera::getMatrix(){
	return camera;
}

} /* namespace AMG */
