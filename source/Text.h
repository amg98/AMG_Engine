/**
 * @file Text.h
 * @brief Text creation and manipulation
 */

#ifndef TEXT_H_
#define TEXT_H_

// Includes OpenGL
#include <glm/glm.hpp>
using namespace glm;

// Own includes
#include "MeshData.h"
#include "Texture.h"

namespace AMG {

/**
 * @class Text
 * @brief Contains text, usable by the engine to show it in 2D mode
 */
class Text: public MeshData {
private:
	Texture *texture;		/**< Texture from a Font object */
public:
	vec3 position;			/**< Text position */
	vec4 color;				/**< Text color */
	float charWidth;		/**< Character width */
	float charEdge;			/**< Character edge */
	float charBorderWidth;	/**< Character border width */
	float charBorderEdge;	/**< Character border edge */
	vec2 charShadowOffset;	/**< Character shadow offset */
	vec3 charOutlineColor;	/**< Character outline color */

	Text(float *vertices, float *texcoords, int size, Texture *texture);
	void draw();
	virtual ~Text();
};

}

#endif
