/*
 * Renderer.h
 *
 *  Created on: 1 jun. 2018
 *      Author: Andrés
 */

#ifndef RENDERER_H_
#define RENDERER_H_

#define GLEW_STATIC
#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
using namespace glm;

#include "Shader.h"
#include "Camera.h"
#include "Entity.h"

namespace AMG {

typedef void (*renderCallback)(void);

class Renderer : private Entity {
private:
	GLFWwindow* window;
	static bool glfwSetup;
	static bool glewSetup;
	renderCallback renderCb;
	mat4 *projection;
	mat4 perspective, ortho;
	mat4 model;
	mat4 mvp;
public:
	int width, height;

	Renderer(int width, int height, const char *title, int resize);
	void update();
	bool running();
	void setCurrent();
	virtual ~Renderer();
	static int exitProcess();
	void setRenderCallback(renderCallback cb);
	void setTransformation(vec3 pos, float angle, vec3 axis, vec3 scale);
	void updateMVP(Shader *shader, Camera *camera);
	void set3dMode(bool mode);
	void calculateProjection();
};

} /* namespace AMG */

#endif /* RENDERER_H_ */
