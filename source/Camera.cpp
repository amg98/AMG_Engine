/** @file Camera.cpp
  * @brief Camera objects
  * It is under development, just a proof of concept
  */

// Includes OpenGL
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/transform.hpp>

// Includes C/C++
#include <math.h>

// Own includes
#include "Camera.h"
#include "Renderer.h"

namespace AMG {

// Views for each side
const static float camViews[] = {
	 0, 			 M_PI/2.0f,
	 0, 		    -M_PI/2.0f,
	-M_PI/2.0f,  	 M_PI,
	 M_PI/2.0f,  	 M_PI,
	 0, 			 M_PI,
	 0,				 0,
};

/**
 * @brief Constructor of a camera
 * @param position Camera's position
 */
Camera::Camera(vec3 position) {
	this->position = position;
	this->camera = glm::translate(mat4(1.0f), -this->position);
	this->rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
	this->rot = vec3(0, 0, 0);
	this->speed = 2.0f;
	this->computeTrihedron();
}

/**
 * @brief Compute the camera trihedron
 */
void Camera::computeTrihedron(){
	forward = glm::normalize(this->rotation * vec3(0, 0, -1));
	right = glm::normalize(this->rotation * vec3(1, 0, 0));
}

/**
 * @brief Update this camera according to a window's input
 * @param window Window to be read its input
 * @param delta Delta time
 * @note Called internally by a renderer
 */
void Camera::update(GLFWwindow *window, float delta){

	// Rotate the camera
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS){
		rot.y -= 3.141592f/2.0f * delta;
	} else if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS){
		rot.y += 3.141592f/2.0f * delta;
	}
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS){
		rot.x += 3.141592f/2.0f * delta;
	} else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS){
		rot.x -= 3.141592f/2.0f * delta;
	}

	// Compute the camera view matrix
	update();

	// Move the camera
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
		this->position += right * delta * speed;
	} else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
		this->position -= right * delta * speed;
	}
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
		this->position += forward * delta * speed;
	} else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){
		this->position -= forward * delta * speed;
	}
}

/**
 * @brief Update the camera matrix
 */
void Camera::update(){
	this->rotation = glm::normalize(glm::angleAxis(rot.z, vec3(0, 0, 1)) * glm::angleAxis(rot.y, vec3(0, 1, 0)) * glm::angleAxis(rot.x, vec3(1, 0, 0)));
	computeTrihedron();
	this->camera = glm::toMat4(glm::conjugate(this->rotation)) * glm::translate(mat4(1.0f), -this->position);
	Renderer::setView(this->camera);
}

/**
 * @brief Get the current ray for this camera
 * @return The direction the user points with the mouse
 */
vec3 Camera::getRay(){

	// Get needed matrices
	mat4 invPerspective = Renderer::getInversePerspective();
	mat4 invView = glm::inverse(this->camera);

	// Get mouse position, in OpenGL coordinates
	double x, y;
	Renderer::getMousePosition(&x, &y);

	// Perform the inverse transformations
	vec4 pos = vec4((2.0f * x) / Renderer::getWidth() - 1, -(2.0f * y) / Renderer::getHeight() + 1, -1, 1);
	pos = invPerspective * pos;
	pos.z = -1;
	pos.w = 0;
	pos = invView * pos;

	// Return a 3D normalised ray vector
	vec3 ray = vec3(pos);
	return glm::normalize(ray);
}

/**
 * @brief Tells the camera where to look
 * @param side Which side to look
 */
void Camera::lookAt(int side){
	side %= 6;		// Prevents an overflow
	rot.x = camViews[(side << 1) + 0];
	rot.y = camViews[(side << 1) + 1];
}

}
