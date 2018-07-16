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

// Includes Windows
#include <windows.h>

namespace AMG {

// Static variables
bool Renderer::glfwSetup = false;
bool Renderer::glewSetup = false;
Renderer *Renderer::currentRenderer = NULL;
GLuint Renderer::quadID;
GLuint Renderer::quadVertices;
GLuint Renderer::quadTexcoords;

static float spr_vertices[] = {				/**< A vertex buffer for a quad (used for particles and 2D rendering) */
	-0.5f, -0.5f,
	0.5f, -0.5f,
	0.5f, 0.5f,
	-0.5f, -0.5f,
	0.5f, 0.5f,
	-0.5f, 0.5f
};

static float uv_vertices[] = {				/**< The quad's UV coordinates */
	0, 1,
	1, 1,
	1, 0,
	0, 1,
	1, 0,
	0, 0
};

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
 */
Renderer::Renderer(int width, int height, const char *title, bool resize, bool fullscreen) {

	// Initialise variables
	this->model = glm::mat4(1.0f);
	this->mvp = glm::mat4(1.0f);
	this->mv = glm::mat4(1.0f);
	this->camera = NULL;
	this->width = width;
	this->height = height;
	this->renderCb = NULL;
	this->FPS = 60.0f;
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
	glfwSwapInterval(1);		// V-sync always enabled

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
		glEnable(GL_MULTISAMPLE);
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

	// Create the sample quad for 2D drawing and particles
	glGenVertexArrays(1, &quadID);
	glBindVertexArray(quadID);
	glGenBuffers(1, &quadVertices);
	glBindBuffer(GL_ARRAY_BUFFER, quadVertices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(spr_vertices), spr_vertices, GL_STATIC_DRAW);
	glGenBuffers(1, &quadTexcoords);
	glBindBuffer(GL_ARRAY_BUFFER, quadTexcoords);
	glBufferData(GL_ARRAY_BUFFER, sizeof(uv_vertices), uv_vertices, GL_STATIC_DRAW);
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

	double b = glfwGetTime();
	double lastframe = b;
	int frames = 0;

	while(running){

		b = glfwGetTime();

		// Perform frame counter
		if(b - lastframe > 1.0){
			FPS = frames;
			frames = 0;
			lastframe = b;
			//printf("%d\n", FPS);
			//fflush(stdout);
		}

		// Process events
		glfwPollEvents();
		if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS || glfwWindowShouldClose(window)){
			running = false;
		}

		// Render the scene
		glClearColor(fogColor.r, fogColor.g, fogColor.b, fogColor.a);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		if(renderCb){
			renderCb();
		}

		glfwSwapBuffers(window);
		frames ++;
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
	glDeleteBuffers(1, &quadVertices);
	glDeleteBuffers(1, &quadTexcoords);
	glDeleteVertexArrays(1, &quadID);

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
 * @brief Calculate model matrix, making the transformed vertices to look towards the camera
 * @param pos Position vector
 * @param rot Rotation in radians
 * @param scale Scale, 1.0f is the default size
 * @note We need a camera set to call this
 */
void Renderer::setTransformationBillboard(vec3 pos, float rot, float scale){
	if(camera){		// We need a camera set
		mat4 &view = camera->getMatrix();
		model = glm::translate(mat4(1.0f), pos);
		model[0][0] = view[0][0];
		model[0][1] = view[1][0];
		model[0][2] = view[2][0];
		model[1][0] = view[0][1];
		model[1][1] = view[1][1];
		model[1][2] = view[2][1];
		model[2][0] = view[0][2];
		model[2][1] = view[1][2];
		model[2][2] = view[2][2];
		model *= glm::rotate(rot, vec3(0, 0, 1)) * glm::scale(vec3(scale, scale, scale));
	}
}

/**
 * @brief Flush model-view-projection matrix to the shader
 * @note Called internally when an Object needs to be rendered
 */
void Renderer::updateMVP(){
	if(camera){
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
 * @brief Flush model-view-projection matrix to a float buffer
 * @param data Float buffer
 * @param offset Where to start storing the matrix (in floats)
 */
void Renderer::storeMVP(float *data, int offset){
	memcpy(&data[offset], &mvp[0][0], 16 * sizeof(float));
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
		glDisable(GL_CULL_FACE);
	}
}

/**
 * @brief Set a current Camera for this Renderer, and updates it
 */
void Renderer::setCamera(Camera *camera){
	this->camera = camera;
	if(camera) camera->update(window, getDelta());
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

/**
 * @brief Get whether a key has been pressed
 * @param code The key code
 */
bool Renderer::getKey(int code){
	return (glfwGetKey(window, code) == GLFW_PRESS);
}

}
