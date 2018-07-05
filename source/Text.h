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
	Text(float *vertices, float *texcoords, int size, Texture *texture);
	void draw();
	virtual ~Text();
};

}

#endif
