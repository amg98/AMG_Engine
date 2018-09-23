/**
 * @file WaterTile.cpp
 * @brief Realistic water effects
 */

// Own includes
#include "Renderer.h"
#include "WaterTile.h"
#include "DeferredRendering.h"

namespace AMG {

// Internal data
static float water_vertices[] = { -1, -1, -1, 1, 1, -1, 1, -1, -1, 1, 1, 1 };	/**< Water vertices, only XZ coordinates */
Shader *WaterTile::waterShader = NULL;

/**
 * @brief Initialize the water engine
 * @param light Light source for water shininess
 */
void WaterTile::initialize(Light *light){

	// Load the shader
	waterShader = new Shader("Effects/AMG_Water.vs", "Effects/AMG_Water.fs");
	waterShader->defineUniform("moveFactor");
	waterShader->getLights().push_back(light);
}

/**
 * @brief Finish the water engine
 */
void WaterTile::finish(){
	AMG_DELETE(waterShader);
}

/**
 * @brief Constructor for a Water tile
 * @param dudvpath Path of the DuDv texture
 * @param pos Water tile position
 * @param size Water tile size
 */
WaterTile::WaterTile(const char *normalMapPath, const char *dudvpath, vec3 pos, float size) {

	// Create the mesh data
	glGenVertexArrays(1, &id);
	glBindVertexArray(id);
	glGenBuffers(1, &bufId);
	glBindBuffer(GL_ARRAY_BUFFER, bufId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(water_vertices), water_vertices, GL_STATIC_DRAW);
	position = pos;
	scale = vec3(size, size, size);

	// Create the framebuffers
	reflection = new Framebuffer();
	reflection->createColorTexture(0);
	refraction = new Framebuffer();
	refraction->createColorTexture(0);

	// Load textures
	dudv = new Texture(dudvpath);
	normalMap = new Texture(normalMapPath);

	// Initialize variables
	waveSpeed = 0.03f;
	moveFactor = 0.0f;
}

/**
 * @brief Prepare the water engine for this frame
 * @param render Rendering callback
 * @note Don't use clipping in skyboxes
 * @note Call this using the default framebuffer
 */
void WaterTile::prepare(AMG_WaterFunctionCallback render){

	// Set the camera for the reflection texture
	Camera *cam = Renderer::getCamera();
	float distance = 2.0f * (cam->getPosition().y - position.y);
	cam->getPosition().y -= distance;
	cam->getRotation().x *= -1.0f;
	cam->getRotation().z *= -1.0f;
	cam->update();

	// Render the reflection texture
	reflection->start();
	render(vec4(0, 1, 0, -position.y));
	reflection->end();

	// Restore the camera
	cam->getPosition().y += distance;
	cam->getRotation().x *= -1.0f;
	cam->getRotation().z *= -1.0f;
	cam->update();

	// Render the refraction texture
	refraction->start();
	render(vec4(0, -1, 0, position.y));
	refraction->end();
}

/**
 * @brief Draw a water tile
 */
void WaterTile::draw(){

	// Set transformations
	waterShader->enable();
	Renderer::setTransformation(position, scale);
	Renderer::updateMVP();
	if(!Renderer::isBBoxVisible(vec3(1, 0.05f, 1))) return;

	// Perform the wave movement
	moveFactor += waveSpeed * Renderer::getDelta();
	float d;
	moveFactor = modf(moveFactor, &d);
	waterShader->setUniform("moveFactor", moveFactor);

	// Bind the needed textures
	reflection->getColorTexture(0)->bind(0);
	refraction->getColorTexture(0)->bind(1);
	dudv->bind(2);
	normalMap->bind(3);

	// Draw the water quad
	glBindVertexArray(id);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, bufId);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glDisableVertexAttribArray(0);
}

/**
 * @brief Destructor for a Water tile
 */
WaterTile::~WaterTile() {
	glDeleteBuffers(1, &bufId);
	glDeleteVertexArrays(1, &id);
	AMG_DELETE(reflection);
	AMG_DELETE(refraction);
	AMG_DELETE(dudv);
	AMG_DELETE(normalMap);
}

}
