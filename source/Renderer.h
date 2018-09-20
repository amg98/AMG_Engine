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
#include "World.h"

/**< Delete an Entity */
#define AMG_DELETE(x) if((x)) delete (x)

namespace AMG {

/**
 * @typedef renderCallback
 * @brief Callback used to render stuff
 */
typedef void (*AMG_FunctionCallback)(void);

/**
 * @class Renderer
 * @brief Static class that holds a window capable of rendering 3D stuff
 */
class Renderer {
private:
	static GLFWwindow* window;					/**< Internal window object */
	static bool init;							/**< The engine has been initialized? */
	static AMG_FunctionCallback renderCb;		/**< Rendering callback */
	static AMG_FunctionCallback unloadCb;		/**< Unload callback */
	static mat4 *projection;					/**< Current projection matrix */
	static mat4 perspective, ortho;				/**< Precalculated perspective and ortho matrices */
	static mat4 view;							/**< View matrix */
	static mat4 invPerspective;					/**< Inverse perspective matrix, for ray tracing */
	static mat4 model;							/**< Model matrix */
	static mat4 mvp;							/**< Concatenation of Model, Projection and Camera matrices */
	static mat4 mv;								/**< Model view matrix */
	static Camera *camera;						/**< Current set camera */
	static int width;							/**< Window width, in pixels */
	static int height;							/**< Window height, in pixels */
	static int FPS;								/**< The actual frames per second */
	static float fogDensity;					/**< Fog density */
	static float fogGradient;					/**< Fog gradient */
	static vec4 fogColor;						/**< Fog color, same as clear color */
	static Shader *currentShader;				/**< Current used shader */
	static mat4 zupConversion;					/**< Matrix to use with Z up coordinate systems */
	static World *world;						/**< Physics world, optional */
	static GLuint quadID;						/**< OpenGL buffer ID for a quad */
	static GLuint quadVertices;					/**< OpenGL buffer ID for the quad's vertices */
	static GLuint quadTexcoords;				/**< OpenGL buffer ID for the quad's texture coordinates */
	static float fov;							/**< Field of view, in radians */
	static float renderDistance;				/**< Maximum render distance */
	static float hdrExposure;					/**< Exposure for the HDR -> LDR color transformation */
	static float gammaCorrection;				/**< Gamma correction factor (2.2 by default) */
	Renderer(){}
public:
	static bool initialized(){ return init; }
	static int getWidth(){ return width; }
	static int getHeight(){ return height; }
	static int getFPS(){ return FPS; }
	static float &getFogDensity(){ return fogDensity; }
	static float &getFogGradient(){ return fogGradient; }
	static vec4 &getFogColor(){ return fogColor; }
	static double getDelta(){ return 1.0f / FPS; }
	static mat4 &getPerspective(){ return perspective; }
	static mat4 &getOrtho(){ return ortho; }
	static mat4 &getInversePerspective(){ return invPerspective; }
	static Shader *getCurrentShader(){ return currentShader; }
	static World *getWorld(){ return world; }
	static float getFOV(){ return fov; }
	static float getAspectRatio(){ return (float)width / (float)height; }
	static void setProjection(mat4 *m){ projection = m; }
	static void setPerspective(){ projection = &perspective; }
	static void setOrtho(){ projection = &ortho; }
	static void setView(mat4 &v){ view = v; }
	static void setCurrentShader(Shader *shader){ currentShader = shader; }
	static void setRenderCallback(AMG_FunctionCallback cb){ renderCb = cb; }
	static void setUnloadCallback(AMG_FunctionCallback cb){ unloadCb = cb; }
	static Camera *getCamera(){ return camera; }
	static void setRenderDistance(float distance){ renderDistance = distance; }
	static mat4 &getView(){ return view; }
	static float &getHDRExposure(){ return hdrExposure; }
	static float &getGammaCorrection(){ return gammaCorrection; }

	static int exitProcess();
	static Texture *createCubeMap(AMG_FunctionCallback render, Shader *shader, int dimensions, vec3 position);

	static void initialize(int w, int h, const char *title, bool fullscreen, int samples=4);
	static void update();
	static void setTransformationZ(vec3 pos, quat rot, vec3 scale);
	static void setTransformation(vec3 pos, quat rot, vec3 scale);
	static void setTransformation(vec3 pos);
	static void setTransformation(vec3 pos, vec3 scale);
	static void setTransformationBillboard(vec3 pos, float rot, float scale);
	static void updateMVP();
	static void storeMVP(float *data, int offset);
	static void set3dMode(bool mode);
	static void calculateProjection();
	static void calculatePanoramicProjection();
	static void updateCamera(Camera *cam);
	static void updateFog();
	static void updateLighting();
	static void getMousePosition(double *x, double *y);
	static bool getKey(int code);
	static void createWorld();
	static void resize(int w, int h);
	static void bindQuad(bool vao);
	static void setFOV(float fieldOfView);
	static bool isBBoxVisible(vec3 box);
};

}

#endif
