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
	vec3 position;			/**< Text position */
	vec4 color;				/**< Text color */
	float charWidth;		/**< Character width */
	float charEdge;			/**< Character edge */
	float charBorderWidth;	/**< Character border width */
	float charBorderEdge;	/**< Character border edge */
	vec2 charShadowOffset;	/**< Character shadow offset */
	vec3 charOutlineColor;	/**< Character outline color */
public:

	vec3 &getPosition(){ return position; }
	vec4 &getColor(){ return color; }
	float &getCharWidth(){ return charWidth; }
	float &getCharEdge(){ return charEdge; }
	float &getCharBorderWidth(){ return charBorderWidth; }
	float &getCharBorderEdge(){ return charBorderEdge; }
	vec2 &getCharShadowOffset(){ return charShadowOffset; }
	vec3 &getCharOutlineColor(){ return charOutlineColor; }


	Text(float *vertices, float *texcoords, int size, Texture *texture);
	void draw();
	virtual ~Text();
};

}

#endif
