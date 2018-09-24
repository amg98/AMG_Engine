/*
 * @file LensFlare.cpp
 * @brief Utilities to draw a lens flare
 */

// Includes C/C++
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Own includes
#include "LensFlare.h"
#include "Renderer.h"

namespace AMG {

/**
 * @brief Constructor for a LensFlare
 * @param dir Directory where the textures are
 * @param spacing Spacing between each texture
 * @param scale Scale array for each lens flare texture
 */
LensFlare::LensFlare(const char *dir, float spacing, float *scale) {

	// Load the textures
	char path[256];
	for(int i=0;i<AMG_LENS_FLARE_TEXTURES;i++){
		sprintf(path, "%s/tex%d.dds", dir, i);
		textures[i] = new Texture(path);
	}

	// Load the sprite
	sprite = new Sprite();

	// Create a query object
	glGenQueries(1, &queryID);

	// Fill information
	this->lens_scale = scale;
	this->spacing = spacing;
	this->query = false;
	this->coverage = 0.0f;
	this->nsamples = textures[0]->getWidth() * textures[0]->getHeight() * lens_scale[0] * lens_scale[0];
}

/**
 * @brief Render the lens flare effect, using 2D quads
 * @param cam Camera used for this effect
 * @param light Light that creates the effect
 */
void LensFlare::draw(Camera *cam, Light *light){

	// Get needed matrices and vectors
	mat4 &view = cam->getMatrix();
	mat4 &proj = Renderer::getPerspective();
	vec3 &pos = light->getPosition();
	float width = Renderer::getWidth();
	float height = Renderer::getHeight();

	// Calculate the light's position, in screen space
	vec4 coords = vec4(pos.x, pos.y, pos.z, 1.0f);
	coords = proj * view * coords;
	if(coords.w <= 0.0f) return;
	coords.x = (coords.x / coords.w + 1.0f) / 2.0f * width;
	coords.y = (coords.y / coords.w + 1.0f) / 2.0f * height;
	vec2 sunToCenter = vec2(width/2.0f - coords.x, height/2.0f - coords.y);
	vec2 sunToCenterNDC = vec2(sunToCenter.x / width, sunToCenter.y / height);

	// Check the brightness is positive
	float brightness = 1.0f - glm::length(sunToCenterNDC) / 0.6f;
	if(brightness > 0.0f){

		// Begin the query
		if(!query){
			glEnable(GL_DEPTH_TEST);
			glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
			glBeginQuery(GL_SAMPLES_PASSED, queryID);
			query = true;
			drawTexture(0, brightness, coords, sunToCenter);
			glEndQuery(GL_SAMPLES_PASSED);
			glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
			glDisable(GL_DEPTH_TEST);
		}

		// Do a query fetch to see how many samples passed the depth test
		int result;
		glGetQueryObjectiv(queryID, GL_QUERY_RESULT_AVAILABLE, &result);
		if(result == GL_TRUE){
			glGetQueryObjectiv(queryID, GL_QUERY_RESULT, &result);
			query = false;
			coverage = result / nsamples;
		}

		// Enable additive blending
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);

		// Draw each texture
		for(int i=0;i<AMG_LENS_FLARE_TEXTURES;i++){
			drawTexture(i, brightness * coverage, coords, sunToCenter);
		}

		// Restore blending settings
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}
}

/**
 * @brief Draw a texture for this effect
 * @param i Which texture to draw
 * @param brightness Alpha channel
 * @param coords Coordinates in the screen
 * @param sunToCenter Screen center - coords
 */
void LensFlare::drawTexture(int i, float brightness, vec4 &coords, vec2 &sunToCenter){
	vec2 direction = sunToCenter * (float)i * spacing;
	sprite->getScaleX() = lens_scale[i];
	sprite->getScaleY() = lens_scale[i];
	sprite->getPosition().x = coords.x + direction.x;
	sprite->getPosition().y = coords.y + direction.y;
	sprite->getColor().a = brightness;
	sprite->set(textures[i]);
	sprite->draw();
}

/**
 * @brief Destructor for a LensFlare
 */
LensFlare::~LensFlare() {
	for(int i=0;i<AMG_LENS_FLARE_TEXTURES;i++){
		delete textures[i];
	}
	glDeleteQueries(1, &queryID);
	AMG_DELETE(sprite);
}

}
