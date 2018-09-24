/**
 * @file Renderer.cpp
 * @brief General rendering stuff
 */

// Includes C/C++
#include <math.h>

// Includes OpenGL
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Own includes
#include "Renderer.h"
#include "Debug.h"
#include "Framebuffer.h"

namespace AMG {

// Static variables
GLFWwindow* Renderer::window;
bool Renderer::init = false;
AMG_FunctionCallback Renderer::renderCb;
AMG_FunctionCallback Renderer::render2dCb;
AMG_FunctionCallback Renderer::unloadCb;
mat4 *Renderer::projection;
mat4 Renderer::perspective, Renderer::ortho;
mat4 Renderer::invPerspective;
mat4 Renderer::model;
mat4 Renderer::mvp;
mat4 Renderer::mv;
mat4 Renderer::view;
int Renderer::width;
int Renderer::height;
int Renderer::FPS;
float Renderer::fogDensity;
float Renderer::fogGradient;
vec4 Renderer::fogColor;
float Renderer::fov;
Camera *Renderer::camera;
Shader *Renderer::currentShader;
Shader *Renderer::hdrGammaShader;
mat4 Renderer::zupConversion;
World *Renderer::world;
GLuint Renderer::quadID;
GLuint Renderer::quadVertices;
GLuint Renderer::quadTexcoords;
float Renderer::renderDistance;
float Renderer::hdrExposure;
float Renderer::gammaCorrection;
int Renderer::srgbTextures;
Framebuffer *Renderer::defaultFB;
Sprite *Renderer::fbSprite;

/**< A vertex buffer for a quad (used for particles and 2D rendering) */
static float spr_vertices[] = {
	-0.5f, -0.5f,
	0.5f, -0.5f,
	0.5f, 0.5f,
	-0.5f, -0.5f,
	0.5f, 0.5f,
	-0.5f, 0.5f
};

/**< The quad's UV coordinates */
static float uv_vertices[] = {
	0, 1,
	1, 1,
	1, 0,
	0, 1,
	1, 0,
	0, 0
};

/**
 * @brief Constructor for a Renderer object
 * @param w Window width, in pixels
 * @param h Window height, in pixels
 * @param title Title for this new window
 * @param fullscreen Full screen window?
 * @param samples Number of samples per pixel
 */
void Renderer::initialize(int w, int h, const char *title, bool fullscreen, int samples) {

	// If it was initialised
	if(init) return;

	// Initialise variables
	width = w;
	height = h;
	renderCb = NULL;
	render2dCb = NULL;
	unloadCb = NULL;
	FPS = 60.0f;
	fogColor = vec4(0.2f, 0.2f, 0.2f, 1.0f);
	fogDensity = 0.0f;
	fogGradient = 1.0f;
	currentShader = NULL;
	world = NULL;
	fov = glm::radians(45.0f);
	renderDistance = 100.0f;
	hdrExposure = 1.0f;
	gammaCorrection = 2.2f;
	srgbTextures = 1;
	hdrGammaShader = NULL;
	defaultFB = NULL;
	fbSprite = NULL;

	// Initialise GLFW
	if(!glfwInit())
		Debug::showError(1, NULL);

	// Set window hints
	glfwWindowHint(GLFW_SAMPLES, 0);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);					// OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwSwapInterval(1);

	// Create a window
	GLFWmonitor *monitor = fullscreen ? glfwGetPrimaryMonitor() : NULL;
	window = glfwCreateWindow(width, height, title, monitor, NULL);
	if(window == NULL){
		glfwTerminate();
		Debug::showError(2, NULL);
	}

	// Set OpenGL context owner to this renderer
	glfwMakeContextCurrent(window);

	// Initialise GLEW
	if (glewInit() != GLEW_OK) {
		glfwTerminate();
		Debug::showError(3, NULL);
	}

	// Set OpenGL properties
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
	glDisable(GL_CULL_FACE);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_MULTISAMPLE);
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

	// Input configuration
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	// Calculate matrices
	model = mat4(1.0f);
	mvp = mat4(1.0f);
	mv = mat4(1.0f);
	view = mat4(1.0f);
	calculateProjection();
	zupConversion = glm::scale(vec3(1, -1, -1)) * glm::rotate(3.141592f/2, vec3(1, 0, 0));
	set3dMode(true);

	// Create the sample quad for 2D drawing and particles
	glGenVertexArrays(1, &quadID);
	glBindVertexArray(quadID);
	glGenBuffers(1, &quadVertices);
	glBindBuffer(GL_ARRAY_BUFFER, quadVertices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(spr_vertices), spr_vertices, GL_STATIC_DRAW);
	glGenBuffers(1, &quadTexcoords);
	glBindBuffer(GL_ARRAY_BUFFER, quadTexcoords);
	glBufferData(GL_ARRAY_BUFFER, sizeof(uv_vertices), uv_vertices, GL_STATIC_DRAW);

	// Load shaders
	hdrGammaShader = new Shader("Effects/AMG_HDRGamma");

	// Create the 3D framebuffer
	defaultFB = new Framebuffer(width, height, 1, samples);
	defaultFB->createColorTexture(0, GL_RGB16F, GL_RGB, GL_FLOAT);
	fbSprite = new Sprite(defaultFB->getColorTexture(0));
	fbSprite->getScaleY() = -1.0f;
	fbSprite->getPosition() = vec3(width / 2.0f, height / 2.0f, 0.0f);

	// All set up
	init = true;
}

/**
 * @brief Creates a physics world for this Renderer
 */
void Renderer::createWorld(){
	world = new World();
}


/**
 * @brief Set the renderer's field of view
 * @param fieldOfView Field of view, in radians
 */
void Renderer::setFOV(float fieldOfView){
	fov = fieldOfView;
	perspective = glm::perspective(fov, (float)width/(float)height, 0.1f, 1000.0f);
	invPerspective = glm::inverse(perspective);
}

/**
 * @brief Calculate projection matrices
 * @note Called whenever a window is created or resized
 */
void Renderer::calculateProjection(){
	perspective = glm::perspective(fov, (float)width/(float)height, 0.1f, 1000.0f);
	invPerspective = glm::inverse(perspective);
	ortho = glm::ortho(0.0f, (float)width, 0.0f, (float)height, -1.0f, 1.0f);
}

/**
 * @brief Calculate the projection matrices for a panoramic view
 */
void Renderer::calculatePanoramicProjection(){
	perspective = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 1000.0f);
}

/**
 * @brief Update the window, flip buffers and read input
 */
void Renderer::update(){

	// Running flag
	bool running = true;

	// Time variables
	double b = glfwGetTime();
	double lastframe = b;
	int frames = 0;

	// Main game loop
	while(running){

		// Get time
		b = glfwGetTime();

		// Perform a frame counter
		if(b - lastframe > 1.0){
			FPS = frames;
			frames = 0;
			lastframe = b;
			/*printf("%d\n", FPS);
			fflush(stdout);*/
		}

		// Process events
		glfwPollEvents();
		if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS || glfwWindowShouldClose(window)){
			running = false;
		}

		// Render the 3D scene onto the framebuffer
		glClearColor(fogColor.r, fogColor.g, fogColor.b, fogColor.a);
		defaultFB->start();
		if(renderCb) renderCb();
		defaultFB->end();
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		// Render the scene
		set3dMode(false);
		hdrGammaShader->enable();
		hdrGammaShader->setUniform(AMG_HDRExposure, Renderer::getHDRExposure());
		hdrGammaShader->setUniform(AMG_GammaValue, Renderer::getGammaCorrection());
		fbSprite->draw();

		if(render2dCb){
			render2dCb();
		}
		set3dMode(true);

		glfwSwapBuffers(window);
		frames ++;

		// Update the physics world
		if(world) world->update(getDelta());
	}
}

/**
 * @brief Static method to finish the engine and exit the program
 * @return A successful exit status (0)
 * @note Entities are destroyed here
 */
int Renderer::exitProcess(){

	// If the engine was initialized
	if(init){
		// Destroy the window
		if(window) glfwDestroyWindow(window);
		if(world) delete world;
		if(hdrGammaShader) delete hdrGammaShader;
		if(fbSprite) delete fbSprite;
		if(defaultFB) delete defaultFB;

		// Delete the quad object
		glDeleteBuffers(1, &quadVertices);
		glDeleteBuffers(1, &quadTexcoords);
		glDeleteVertexArrays(1, &quadID);

		// Unload data
		if(unloadCb) unloadCb();
		if(Entity::nEntities > 0){
			fprintf(stderr, "Warning: %d resources were not unloaded\n", Entity::nEntities);
			fflush(stderr);
		}

		// Terminate GLFW
		glfwTerminate();
		init = false;
	}

	// Return 0
	return 0;
}

/**
 * @brief Calculate model matrix from translation, rotation and scale vectors (Z up axis)
 * @param pos Position vector
 * @param rot Rotation quaternion
 * @param scale Vector of scale (1.0f, 1.0f, 1.0f) is default size
 * @note This coordinate system is used in programs like Blender
 */
void Renderer::setTransformationZ(vec3 pos, quat rot, vec3 scale){
	model = glm::translate(mat4(1.0f), pos) * glm::toMat4(rot) * glm::scale(scale) * zupConversion;
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
 * @brief Calculate model matrix from translation and scale vectors
 * @param pos Position vector
 * @param scale Vector of scale (1.0f, 1.0f, 1.0f) is default size
 */
void Renderer::setTransformation(vec3 pos, vec3 scale){
	model = glm::translate(mat4(1.0f), pos) * glm::scale(scale);
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
 * @note We need a set camera to call this
 */
void Renderer::setTransformationBillboard(vec3 pos, float rot, float scale){
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

/**
 * @brief Flush model-view-projection matrix to the shader
 * @note Called internally when an Object needs to be rendered
 */
void Renderer::updateMVP(){
	mv = view * model;
	mvp = *projection * mv;
	if(camera) currentShader->setUniform(AMG_CamPosition, camera->getPosition());
	currentShader->setUniform(AMG_MVP, mvp);
	currentShader->setUniform(AMG_MV, mv);
	currentShader->setUniform(AMG_M, model);
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
		projection = &perspective;
		glEnable(GL_DEPTH_TEST);
		glDepthMask(GL_TRUE);
	}else{
		projection = &ortho;
		glDisable(GL_DEPTH_TEST);
		glDepthMask(GL_FALSE);
		glDisable(GL_CULL_FACE);
		view = mat4(1.0f);
	}
}

/**
 * @brief Update a camera in the Renderer
 * @param cam Camera to be set
 */
void Renderer::updateCamera(Camera *cam){
	camera = cam;
	cam->update(window, getDelta());
}

/**
 * @brief Update fog to the current shader
 */
void Renderer::updateFog(){
	currentShader->setUniform(AMG_FogColor, fogColor);
	currentShader->setUniform(AMG_FogDensity, fogDensity);
	currentShader->setUniform(AMG_FogGradient, fogGradient);
}

/**
 * @brief Update lighting to the current shader
 */
void Renderer::updateLighting(){
	std::vector<Light*> lights = currentShader->getLights();
	for(unsigned int i=0;i<lights.size();i++){
		lights[i]->enable(i);
	}
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

/**
 * @brief Create a cube map texture from a scene
 * @param render Callback which draws the scene
 * @param shader Shader to draw the cube map (a simple texture is recommended)
 * @param dimensions Dimensions (width and height) of the cube map
 * @param position Where to locate the camera
 * @return A cube map texture with the scene in it
 */
Texture *Renderer::createCubeMap(AMG_FunctionCallback render, Shader *shader, int dimensions, vec3 position){

	// Enable the shader
	shader->enable();

	// Calculate the view and projection matrices
	Camera *cam = new Camera(position);
	cam->getRotation().z = M_PI;
	calculatePanoramicProjection();

	// Create a cube map texture
	Texture *texture = new Texture();
	texture->createCubeMap(dimensions);

	// Create a framebuffer
	Framebuffer *fb = new Framebuffer(dimensions, dimensions);
	fb->start();

	// Set the clear color
	glClearColor(fogColor.r, fogColor.g, fogColor.b, fogColor.a);

	// Render each side of the cube map
	for(int i=0;i<AMG_CUBE_SIDES;i++){
		// Select the appropiate face
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, texture->getID(), 0);
		// Setup the camera
		cam->lookAt(i);
		updateCamera(cam);
		// Render the scene
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		render();
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, 0);
	}

	// Restore the projection matrix
	calculateProjection();

	// Set the framebuffer to the default one
	fb->unbind();

	// Delete stuff
	AMG_DELETE(fb);
	AMG_DELETE(cam);

	// Return the cube map texture
	return texture;
}

/**
 * @brief Resize the window
 * @param w New width, in pixels
 * @param h new height, in pixels
 */
void Renderer::resize(int w, int h){
	if(w > 0 && h > 0){
		width = w;
		height = h;
		calculateProjection();
		glViewport(0, 0, width, height);
	}
}

/**
 * @brief Bind the quad object
 * @param vao Bind the VAO?
 */
void Renderer::bindQuad(bool vao){
	if(vao) glBindVertexArray(quadID);
	glBindBuffer(GL_ARRAY_BUFFER, quadVertices);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindBuffer(GL_ARRAY_BUFFER, quadTexcoords);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, NULL);
}

/**
 * @brief Checks whether a bounding box is visible on screen
 * @param box Maximum coordinate of the bounding box
 */
bool Renderer::isBBoxVisible(vec3 box){

	// Get the bounding box
	vec4 p[8];
	p[0] = vec4( box.x,  box.y,  box.z, 1.0f);
	p[1] = vec4(-box.x,  box.y,  box.z, 1.0f);
	p[2] = vec4( box.x, -box.y,  box.z, 1.0f);
	p[3] = vec4(-box.x, -box.y,  box.z, 1.0f);
	p[4] = vec4( box.x,  box.y, -box.z, 1.0f);
	p[5] = vec4(-box.x,  box.y, -box.z, 1.0f);
	p[6] = vec4( box.x, -box.y, -box.z, 1.0f);
	p[7] = vec4(-box.x, -box.y, -box.z, 1.0f);

	// Convert to normalized device coordinates
	int maxZ = 0;			// Index of the corner nearer to the camera
	p[0] = mv * p[0];
	for(int i=1;i<8;i++){
		p[i] = mv * p[i];
		if(p[i].z > p[maxZ].z){
			maxZ = i;
		}
	}

	// Check is not so far away from the camera
	if(camera){
		vec4 &modelCoords = p[maxZ];
		float distance = glm::length(vec3(modelCoords.x, modelCoords.y, modelCoords.z) - camera->getPosition());
		if(distance > renderDistance) return false;
	}

	// Check if any vertex is inside the screen
	for(int i=0;i<8;i++){
		p[i] = *projection * p[i];			// Convert to normalized device coordinates
		p[i] /= p[i].w;						// Perform perspective division
		if(p[i].x > -1.0f && p[i].x < 1.0f && p[i].y > -1.0f && p[i].y < 1.0f && p[i].z < 1.0f){
			return true;
		}
	}

	// Check if any edge intersects the screen
	vec4 r[12];
	r[0 ] = p[1] - p[0];
	r[1 ] = p[5] - p[1];
	r[2 ] = p[4] - p[5];
	r[3 ] = p[0] - p[4];
	r[4 ] = p[3] - p[2];
	r[5 ] = p[6] - p[3];
	r[6 ] = p[7] - p[6];
	r[7 ] = p[2] - p[7];
	r[8 ] = p[1] - p[3];
	r[9 ] = p[0] - p[2];
	r[10] = p[5] - p[6];
	r[11] = p[4] - p[7];
	float factor;
	for(int i=0;i<12;i++){
		factor = (-1 -p[i].x) / r[i].x;
		if(factor > 0.0f && factor < 1.0f) return true;
		factor = ( 1 -p[i].y) / r[i].y;
		if(factor > 0.0f && factor < 1.0f) return true;
		factor = ( 1 -p[i].x) / r[i].x;
		if(factor > 0.0f && factor < 1.0f) return true;
		factor = (-1 -p[i].y) / r[i].y;
		if(factor > 0.0f && factor < 1.0f) return true;
	}

	// Not a visible object
	return false;
}

}
