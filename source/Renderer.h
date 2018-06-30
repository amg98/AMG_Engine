/**
 * @file Renderer.h
 * @brief General rendering stuff
 */

#ifndef RENDERER_H_
#define RENDERER_H_

// Includes OpenGL
#define GLEW_STATIC
#define GLM_ENABLE_EXPERIMENTAL
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
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
	renderCallback updateCb;		/**< Update callback */
	mat4 *projection;				/**< Current projection matrix */
	mat4 perspective, ortho;		/**< Precalculated perspective and ortho matrices */
	mat4 model;						/**< Model matrix */
	mat4 mvp;						/**< Concatenation of Model, Projection and Camera matrices */
	mat4 mv;						/**< Model view matrix */
	Camera *camera;					/**< Current set camera */
	double frametime;				/**< Desired frame time*/
public:
	int width;							/**< Window width, in pixels (don't write) */
	int height;							/**< Window height, in pixels (don't write) */
	static Shader *shader;				/**< Current used shader */
	static Renderer *currentRenderer;	/**< Current Renderer drawing */
	int FPS;							/**< The actual frames per second */
	float fogDensity;					/**< Fog density */
	float fogGradient;					/**< Fog gradient */
	vec4 fogColor;						/**< Fog color, same as clear color */

	Renderer(int width, int height, const char *title, bool resize, bool fullscreen, double fps);
	void update();
	void setCurrent();
	virtual ~Renderer();
	static int exitProcess();
	void setRenderCallback(renderCallback cb);
	void setUpdateCallback(renderCallback cb);
	void setTransformation(vec3 pos, quat rot, vec3 scale);
	void updateMVP();
	void set3dMode(bool mode);
	void calculateProjection();
	void setCamera(Camera *camera);
	void updateFog();
	double getDelta();
};

}

#endif
