/**
 * @file Renderer.cpp
 * @brief General rendering stuff
 */

// Includes OpenGL
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Own includes
#include "Renderer.h"
#include "Debug.h"

namespace AMG {

bool Renderer::glfwSetup = false;
bool Renderer::glewSetup = false;
Shader *Renderer::shader = NULL;

/**
 * @brief Static method to resize a window
 * @param window Window to be resized
 * @param newWidth New width, in pixels
 * @param newHeight new height, in pixels
 */
static void resizeCallback(GLFWwindow *window, int newWidth, int newHeight){
	Renderer *renderer = (Renderer*) glfwGetWindowUserPointer(window);
	if(renderer){
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
 * @todo Full screen parameter
 */
Renderer::Renderer(int width, int height, const char *title, int resize) {

	// Initialise variables
	this->model = glm::mat4(1.0f);
	this->mvp = glm::mat4(1.0f);
	this->camera = NULL;
	this->width = width;
	this->height = height;

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
	window = glfwCreateWindow(width, height, title, NULL, NULL);
	if(window == NULL){
		glfwTerminate();
		Debug::showError(2, NULL);
	}

	// Set OpenGL context owner to this renderer
	this->setCurrent();

	// Initialise GLEW
	if(!glewSetup){
		if (glewInit() != GLEW_OK) {
			glfwTerminate();
			Debug::showError(3, NULL);
		}
		glEnable(GL_CULL_FACE);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		shader = new Shader("Data/Shader/default.vs", "Data/Shader/default.fs");
		shader->defineUniform("boneMatrix");
		shader->enable();
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
}

/**
 * @brief Calculate projection matrices
 * @note Called whenever a window is created or resized
 */
void Renderer::calculateProjection(){
	this->perspective = glm::perspective(glm::radians(45.0f), (float)this->width/(float)this->height, 0.1f, 100.0f);
	this->ortho = glm::ortho(0.0f, (float)this->width, 0.0f, (float)this->height, -1.0f, 1.0f);
}

/**
 * @brief Update a renderer, flipping buffers and reading input
 */
void Renderer::update(){
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if(renderCb)
		renderCb();

	glfwSwapBuffers(window);
	glfwPollEvents();
}

/**
 * @brief Check whether this renderer is running already
 * @return Whether the renderer is running or should be closed
 */
bool Renderer::running(){
	return (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0);
}

/**
 * @brief Set OpenGL context owner to this renderer
 */
void Renderer::setCurrent(){
	glfwMakeContextCurrent(window);
}

/**
 * @brief Destructor for a Renderer object
 */
Renderer::~Renderer() {
	glfwDestroyWindow(window);
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
 * @brief Calculate model matrix from translation, rotation and scale vectors
 * @param pos Position vector
 * @param angle Angle of rotation, in radians
 * @param axis Axis of rotation, needs to be normalised
 * @param scale Vector of scale (1.0f, 1.0f, 1.0f) is default size
 */
void Renderer::setTransformation(vec3 pos, float angle, vec3 axis, vec3 scale){
	model = glm::translate(mat4(1.0f), pos) * glm::rotate(angle, axis) * glm::scale(scale);
}

/**
 * @brief Flush model-view-projection matrix to the shader
 * @note Called internally when an Object needs to be rendered
 */
void Renderer::updateMVP(){
	if(camera){
		camera->update(this->window);
		mvp = *projection * camera->getMatrix() * model;
	}else{
		mvp = *projection * model;
	}
	glUniformMatrix4fv(shader->getUniform("MVP"), 1, GL_FALSE, &mvp[0][0]);
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
	}
}

/**
 * @brief Set a current Camera for this Renderer
 */
void Renderer::setCamera(Camera *camera){
	this->camera = camera;
}

}
