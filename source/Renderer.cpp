/*
 * Renderer.cpp
 *
 *  Created on: 1 jun. 2018
 *      Author: Andrés
 */

#include "Renderer.h"
#include "Debug.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace AMG {

bool Renderer::glfwSetup = false;
bool Renderer::glewSetup = false;

static void resizeCallback(GLFWwindow *window, int newWidth, int newHeight){
	Renderer *renderer = (Renderer*) glfwGetWindowUserPointer(window);
	if(renderer){
		renderer->width = newWidth;
		renderer->height = newHeight;
		renderer->calculateProjection();
		glViewport(0, 0, newWidth, newHeight);
	}
}

Renderer::Renderer(int width, int height, const char *title, int resize) {
	this->model = glm::mat4(1.0f);
	this->mvp = glm::mat4(1.0f);

	if(!glfwSetup){
		if(!glfwInit())
			Debug::showError(1, NULL);

		glfwWindowHint(GLFW_SAMPLES, 4);
		glfwWindowHint(GLFW_RESIZABLE, resize);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwSetup = true;
	}

	this->width = width;
	this->height = height;

	window = glfwCreateWindow(width, height, title, NULL, NULL);
	if(window == NULL){
		glfwTerminate();
		Debug::showError(2, NULL);
	}

	this->setCurrent();

	if(!glewSetup){
		if (glewInit() != GLEW_OK) {
			glfwTerminate();
			Debug::showError(3, NULL);
		}
		glEnable(GL_CULL_FACE);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glewSetup = true;
	}

	if(resize){
		glfwSetWindowSizeCallback(window, resizeCallback);
	}

	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	glfwSetWindowUserPointer(window, this);

	this->calculateProjection();
	this->set3dMode(true);
}

void Renderer::calculateProjection(){
	this->perspective = glm::perspective(glm::radians(45.0f), (float)this->width/(float)this->height, 0.1f, 100.0f);
	this->ortho = glm::ortho(0.0f, (float)this->width, 0.0f, (float)this->height, -1.0f, 1.0f);
}

void Renderer::update(){
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if(renderCb)
		renderCb();
	glfwSwapBuffers(window);
	glfwPollEvents();
}

bool Renderer::running(){
	return (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0);
}

void Renderer::setCurrent(){
	glfwMakeContextCurrent(window);
}

Renderer::~Renderer() {
	glfwDestroyWindow(window);
}

int Renderer::exitProcess(){
	Entity::destroyEntities();
	if(glfwSetup){
		glfwTerminate();
		glfwSetup = false;
	}
	return 0;
}

void Renderer::setRenderCallback(renderCallback cb){
	this->renderCb = cb;
}

void Renderer::setTransformation(vec3 pos, float angle, vec3 axis, vec3 scale){
	model = glm::translate(mat4(1.0f), pos) * glm::rotate(angle, axis) * glm::scale(scale);
}

void Renderer::updateMVP(Shader *shader, Camera *camera){
	if(camera){
		camera->update(this->window);
		mvp = *projection * camera->getMatrix() * model;
	}else{
		mvp = *projection * model;
	}
	glUniformMatrix4fv(shader->getUniform("MVP"), 1, GL_FALSE, &mvp[0][0]);
}

void Renderer::set3dMode(bool mode){
	if(mode){
		this->projection = &this->perspective;
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
	}else{
		this->projection = &this->ortho;
		glDisable(GL_DEPTH_TEST);
	}
}

} /* namespace AMG */
