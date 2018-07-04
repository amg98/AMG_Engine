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
	this->position = glm::vec3(0, 0, 5);
	this->camera = glm::translate(mat4(1.0f), -this->position);
	this->rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
	this->mode = mode;
	this->rotX = 0.0f;
	this->rotY = 0.0f;
	this->computeTrihedron();
}

/**
 * @brief Destructor of a camera
 */
Camera::~Camera() {

}

/**
 * @brief Update this camera according to a window's input
 * @param window Window to be read its input
 * @param delta Delta time
 * @note Called internally by a renderer
 */
void Camera::update(GLFWwindow *window, float delta){
	switch(this->mode){
		case FPS_CAMERA:
			this->updateFPS(window, delta);
			break;
		default:
			break;
	}
}

/**
 * @brief Compute the camera trihedron
 */
void Camera::computeTrihedron(){
	forward = glm::normalize(this->rotation * vec3(0, 0, 1));
	right = glm::normalize(this->rotation * vec3(1, 0, 0));
}

/**
 * @brief Update a FPS camera
 * @param window Window to be read its input
 * @param delta Delta time
 * @note Called internally by a renderer
 */
void Camera::updateFPS(GLFWwindow *window, float delta){

	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS){
		rotY -= 3.141592f/4.0f * delta;
	} else if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS){
		rotY += 3.141592f/4.0f * delta;
	}
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS){
		rotX += 3.141592f/4.0f * delta;
	} else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS){
		rotX -= 3.141592f/4.0f * delta;
	}
	this->rotation = glm::normalize(glm::angleAxis(rotY, vec3(0, 1, 0)) * glm::angleAxis(rotX, vec3(1, 0, 0)));
	computeTrihedron();
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
		this->position += right * delta;
	} else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
		this->position -= right * delta;
	}
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
		this->position -= forward * delta;
	} else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){
		this->position += forward * delta;
	}

	this->camera = glm::toMat4(glm::conjugate(this->rotation)) * glm::translate(mat4(1.0f), -this->position);
}

/**
 * @brief Get camera matrix
 * @return Camera matrix
 */
mat4& Camera::getMatrix(){
	return camera;
}

/**
 * @brief Get the current ray for this camera
 * @return The direction the user points with the mouse
 */
vec3 Camera::getRay(){

	Renderer *renderer = Renderer::currentRenderer;

	// Get needed matrices
	mat4 invPerspective = renderer->getInversePerspective();
	mat4 invView = glm::inverse(this->camera);

	// Get mouse position, in OpenGL coordinates
	double x, y;
	renderer->getMousePosition(&x, &y);

	// Perform the inverse transformations
	vec4 pos = vec4((2.0f * x)/renderer->width - 1, -(2.0f * y)/renderer->height + 1, -1, 1);
	pos = invPerspective * pos;
	pos.z = -1;
	pos.w = 0;
	pos = invView * pos;

	// Return a 3D normalised ray vector
	vec3 ray = vec3(pos);
	return glm::normalize(ray);
}

}
