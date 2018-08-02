/*
 * @file LensFlare.h
 * @brief Utilities to draw a lens flare
 */

#ifndef LENSFLARE_H_
#define LENSFLARE_H_

// Own includes
#include "Sprite.h"

// Defines
#define AMG_LENS_FLARE_TEXTURES 10		/**< Number of textures in this effect */

namespace AMG {

/**
 * @class LensFlare
 * @brief Holds flare textures to render the effect
 */
class LensFlare : public Entity {
private:
	Texture *textures[AMG_LENS_FLARE_TEXTURES];		/**< 10 lens flare textures */
	Sprite *sprite;									/**< Sprite to draw each texture */
	float spacing;									/**< Spacing between every 2 textures */
	GLuint queryID;									/**< Query ID */
	bool query;										/**< Doing a query? */
	float coverage;									/**< How much occlusion is happening? */
	float nsamples;									/**< Maximum number of samples */
	void drawTexture(int i, float brightness, vec4 &coords, vec2 &sunToCenter);
public:
	LensFlare(const char *dir, float spacing, int nsamples);
	void draw(Camera *cam, Light *light);
	virtual ~LensFlare();
};

}

#endif
