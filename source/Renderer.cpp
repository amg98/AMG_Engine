/**
 * @file Renderer.cpp
 * @brief General rendering stuff
 */

// Includes C/C++
#include <unistd.h>

// Includes OpenGL
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Own includes
#include "Renderer.h"
#include "Debug.h"

// Includes Windows
#include <windows.h>

namespace AMG {

bool Renderer::glfwSetup = false;
bool Renderer::glewSetup = false;
Renderer *Renderer::currentRenderer = NULL;

/**
 * @brief Static method to resize a window
 * @param window Window to be resized
 * @param newWidth New width, in pixels
 * @param newHeight new height, in pixels
 */
static void resizeCallback(GLFWwindow *window, int newWidth, int newHeight){
	Renderer *renderer = (Renderer*) glfwGetWindowUserPointer(window);
	if(renderer && newWidth > 0 && newHeight > 0){
		renderer->width = newWidth;
		renderer->height = newHeight;
		renderer->calculateProjection();
		glViewport(0, 0, newWidth, newHeight);
	}
}

/**
 * @brief Constructor for a Renderer object
 * @param width Window width, in pixels
 * @param height Window height, in pixels
 * @param title Title for this new window
 * @param resize Resizable window?
 * @param fullscreen Full screen window?
 * @param fps Desired frames per second
 */
Renderer::Renderer(int width, int height, const char *title, bool resize, bool fullscreen, double fps) {

	// Initialise variables
	this->model = glm::mat4(1.0f);
	this->mvp = glm::mat4(1.0f);
	this->mv = glm::mat4(1.0f);
	this->camera = NULL;
	this->width = width;
	this->height = height;
	this->frametime = 1.0/fps;
	this->renderCb = NULL;
	this->updateCb = NULL;
	this->FPS = fps;
	this->fogColor = vec4(0.2f, 0.2f, 0.2f, 1.0f);
	this->fogDensity = 0.0f;
	this->fogGradient = 1.0f;
	this->window = NULL;

	// Initialise GLFW
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

	// Create a window
	GLFWmonitor *monitor = (fullscreen && !resize) ? glfwGetPrimaryMonitor() : NULL;
	window = glfwCreateWindow(width, height, title, monitor, NULL);
	if(window == NULL){
		glfwTerminate();
		Debug::showError(2, NULL);
	}

	// Set OpenGL context owner to this renderer
	this->setCurrent();
	glfwSwapInterval(1);			// Vsync enabled by default

	// Initialise GLEW
	if(!glewSetup){
		if (glewInit() != GLEW_OK) {
			glfwTerminate();
			Debug::showError(3, NULL);
		}
		glCullFace(GL_BACK);
		glDisable(GL_CULL_FACE);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glewSetup = true;
	}

	// Set the resize callback, if needed
	if(resize){
		glfwSetWindowSizeCallback(window, resizeCallback);
	}

	// Input configuration
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	glfwSetWindowUserPointer(window, this);

	// Calculate matrices
	this->calculateProjection();
	this->set3dMode(true);

	// Load 2D shader
	shader2d = new Shader("shader2d.vs", "shader2d.fs", AMG_USE_2D);
}

/**
 * @brief Calculate projection matrices
 * @note Called whenever a window is created or resized
 */
void Renderer::calculateProjection(){
	this->perspective = glm::perspective(glm::radians(45.0f), (float)this->width/(float)this->height, 0.1f, 1000.0f);
	this->invPerspective = glm::inverse(this->perspective);
	this->ortho = glm::ortho(0.0f, (float)this->width, 0.0f, (float)this->height, -1.0f, 1.0f);
}

/**
 * @brief Update a renderer, flips buffers, reads input and limits frames per second
 * @note You should use multi-threading to have multiple windows, as the main loop is here
 * @todo Multi-threaded renderers
 */
void Renderer::update(){

	bool running = true;

	double lastTime = glfwGetTime();
	double startTime;
	double passedTime;
	double unprocessedTime = 0;
	double frameCounter = 0;
	int frames = 0;
	bool render;

	// Main loop
	while(running){

		render = false;

		startTime = glfwGetTime();
		passedTime = startTime - lastTime;
		lastTime = startTime;

		unprocessedTime += passedTime;
		frameCounter += passedTime;

		while(unprocessedTime > frametime){
			render = true;
			unprocessedTime -= frametime;

			if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS || glfwWindowShouldClose(window)){
				running = false;
			}

			// Update engine
			glfwPollEvents();
			if(updateCb) updateCb();

			if(frameCounter > 1.0){
				FPS = frames;
				frames = 0;
				frameCounter = 0;
				/*fprintf(stdout, "%d\n", FPS);
				fflush(stdout);*/
			}
		}

		// Render scene
		if(renderCb && render){
			glClearColor(fogColor.r, fogColor.g, fogColor.b, fogColor.a);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			renderCb();
			glfwSwapBuffers(window);
			frames ++;
		}else{
			Sleep(1);
		}
	}
}

/**
 * @brief Set OpenGL context owner to this renderer
 */
void Renderer::setCurrent(){
	glfwMakeContextCurrent(window);
	Renderer::currentRenderer = this;
}

/**
 * @brief Destructor for a Renderer object
 */
Renderer::~Renderer() {
	if(window) glfwDestroyWindow(window);
}

/**
 * @brief Static method to finish the engine and exit the program
 * @return A successful exit status (0)
 * @note Entities are destroyed here
 */
int Renderer::exitProcess(){
	Entity::destroyEntities();
	if(glfwSetup){
		glfwTerminate();
		glfwSetup = false;
	}
	return 0;
}

/**
 * @brief Set a rendering callback to be able to render 3D stuff
 */
void Renderer::setRenderCallback(renderCallback cb){
	this->renderCb = cb;
}

/**
 * @brief Set a update callback to update the engine
 */
void Renderer::setUpdateCallback(renderCallback cb){
	this->updateCb = cb;
}

/**
 * @brief Calculate model matrix from translation, rotation and scale vectors
 * @param pos Position vector
 * @param rot Rotation quaternion
 * @param scale Vector of scale (1.0f, 1.0f, 1.0f) is default size
 */
void Renderer::setTransformation(vec3 pos, quat rot, vec3 scale){
	model = glm::translate(mat4(1.0f), pos) * glm::toMat4(rot) * glm::scale(scale);
}

/**
 * @brief Calculate model matrix from translation vector
 * @param pos Position vector
 */
void Renderer::setTransformation(vec3 pos){
	model = glm::translate(mat4(1.0f), pos);
}

/**
 * @brief Flush model-view-projection matrix to the shader
 * @note Called internally when an Object needs to be rendered
 */
void Renderer::updateMVP(){
	if(camera){
		camera->update(this->window, this->getDelta());
		mv = camera->getMatrix() * model;
	}else{
		mv = model;
	}
	mvp = *projection * mv;
	currentShader->setUniform("AMG_MVP", mvp);
	currentShader->setUniform("AMG_MV", mv);
	currentShader->setUniform("AMG_M", model);
}

/**
 * @brief Change current projection method
 * @param mode true if you want perspective projection, false if you want orthographic projection
 */
void Renderer::set3dMode(bool mode){
	if(mode){
		this->projection = &this->perspective;
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
	}else{
		this->projection = &this->ortho;
		glDisable(GL_DEPTH_TEST);
		this->setCamera(NULL);
		shader2d->enable();
	}
}

/**
 * @brief Set a current Camera for this Renderer
 */
void Renderer::setCamera(Camera *camera){
	this->camera = camera;
}

/**
 * @brief Get the delta value for this Renderer
 * @return The renderer's delta time
 */
double Renderer::getDelta(){
	return 1.0f/FPS;
}

/**
 * @brief Update fog to the current shader
 */
void Renderer::updateFog(){
	currentShader->setUniform("AMG_FogColor", fogColor);
	currentShader->setUniform("AMG_FogDensity", fogDensity);
	currentShader->setUniform("AMG_FogGradient", fogGradient);
}

/**
 * @brief Update lighting to the current shader
 */
void Renderer::updateLighting(){
	for(unsigned int i=0;i<currentShader->lights.size();i++){
		currentShader->lights[i]->enable(i);
	}
}

/**
 * @brief Get the inverse perspective matrix
 * @return The inverse perspective matrix
 */
mat4 &Renderer::getInversePerspective(){
	return this->invPerspective;
}

/**
 * @brief Retrieve mouse position for this Renderer (window)
 * @param x Output X coordinate, in pixels
 * @param y Output Y coordinate, in pixels
 */
void Renderer::getMousePosition(double *x, double *y){
	glfwGetCursorPos(window, x, y);
}

}
