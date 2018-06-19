/**
 * @file Renderer.h
 * @brief General rendering stuff
 */

#ifndef RENDERER_H_
#define RENDERER_H_

// Includes OpenGL
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
using namespace glm;

// Own includes
#include "Shader.h"
#include "Camera.h"
#include "Entity.h"

namespace AMG {

/**
 * @typedef renderCallback
 * @brief Callback used to render stuff
 */
typedef void (*renderCallback)(void);

/**
 * @class Renderer
 * @brief Class that holds a window capable of rendering 3D stuff
 */
class Renderer : private Entity {
private:
	GLFWwindow* window;				/**< Internal window object */
	static bool glfwSetup;			/**< Whether GLFW is setup */
	static bool glewSetup;			/**< Whether GLEW is setup */
	renderCallback renderCb;		/**< Rendering callback */
	mat4 *projection;				/**< Current projection matrix */
	mat4 perspective, ortho;		/**< Precalculated perspective and ortho matrices */
	mat4 model;						/**< Model matrix */
	mat4 mvp;						/**< Concatenation of Model, Projection and Camera matrices */
	Camera *camera;					/**< Current set camera */
public:
	int width;						/**< Window width, in pixels (don't write) */
	int height;						/**< Window height, in pixels (don't write) */
	static Shader *shader;			/**< Current used shader */

	Renderer(int width, int height, const char *title, int resize);
	void update();
	bool running();
	void setCurrent();
	virtual ~Renderer();
	static int exitProcess();
	void setRenderCallback(renderCallback cb);
	void setTransformation(vec3 pos, float angle, vec3 axis, vec3 scale);
	void updateMVP();
	void set3dMode(bool mode);
	void calculateProjection();
	void setCamera(Camera *camera);
};

}

#endif
