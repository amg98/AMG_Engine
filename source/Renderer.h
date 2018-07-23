/**
 * @file Renderer.h
 * @brief General rendering stuff
 */

#ifndef RENDERER_H_
#define RENDERER_H_

// Includes OpenGL
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
#include "World.h"

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
class Renderer : public Entity {
private:
	GLFWwindow* window;				/**< Internal window object */
	static bool glfwSetup;			/**< Whether GLFW is setup */
	static bool glewSetup;			/**< Whether GLEW is setup */
	renderCallback renderCb;		/**< Rendering callback */
	mat4 *projection;				/**< Current projection matrix */
	mat4 perspective, ortho;		/**< Precalculated perspective and ortho matrices */
	mat4 invPerspective;			/**< Inverse perspective matrix, for ray tracing */
	mat4 model;						/**< Model matrix */
	mat4 mvp;						/**< Concatenation of Model, Projection and Camera matrices */
	mat4 mv;						/**< Model view matrix */
	int width;						/**< Window width, in pixels (don't write) */
	int height;						/**< Window height, in pixels (don't write) */
	int FPS;						/**< The actual frames per second */
	float fogDensity;				/**< Fog density */
	float fogGradient;				/**< Fog gradient */
	vec4 fogColor;					/**< Fog color, same as clear color */
	Camera *camera;					/**< Current set camera */
	Shader *currentShader;			/**< Current used shader */
	mat4 zupConversion;				/**< Matrix to use with Z up coordinate systems */
	World *world;					/**< Physics world, optional */
public:
	int &getWidth(){ return width; }
	int &getHeight(){ return height; }
	int getFPS(){ return FPS; }
	float &getFogDensity(){ return fogDensity; }
	float &getFogGradient(){ return fogGradient; }
	vec4 &getFogColor(){ return fogColor; }
	double getDelta(){ return 1.0f / FPS; }
	mat4 &getInversePerspective(){ return invPerspective; }
	Camera *getCamera(){ return camera; }
	Shader *getCurrentShader(){ return currentShader; }
	World *getWorld(){ return world; }
	void setCurrentShader(Shader *shader){ currentShader = shader; }

	static Renderer *currentRenderer;	/**< Current renderer drawing */
	static GLuint quadID;				/**< OpenGL buffer ID for a quad */
	static GLuint quadVertices;			/**< OpenGL buffer ID for the quad's vertices */
	static GLuint quadTexcoords;		/**< OpenGL buffer ID for the quad's texture coordinates */

	static int exitProcess();
	static Texture *createCubeMap(renderCallback render, Shader *shader, int dimensions, vec3 position);

	Renderer(int width, int height, const char *title, bool resize, bool fullscreen, int samples=4);
	void update();
	void setCurrent();
	virtual ~Renderer();
	void setRenderCallback(renderCallback cb);
	void setTransformationZ(vec3 pos, quat rot, vec3 scale);
	void setTransformation(vec3 pos, quat rot, vec3 scale);
	void setTransformation(vec3 pos);
	void setTransformationBillboard(vec3 pos, float rot, float scale);
	void updateMVP();
	void storeMVP(float *data, int offset);
	void set3dMode(bool mode);
	void calculateProjection();
	void calculatePanoramicProjection();
	void setCamera(Camera *camera);
	void updateFog();
	void updateLighting();
	void getMousePosition(double *x, double *y);
	bool getKey(int code);
	void createWorld();
	void updateReflections(Texture *cubeMap, int slot);
};

}

#endif
