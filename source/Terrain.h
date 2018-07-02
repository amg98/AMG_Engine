/**
 * @file Terrain.h
 * @brief Description of terrain objects
 */

#ifndef TERRAIN_H_
#define TERRAIN_H_

// Own includes
#include "Object.h"
#include "Light.h"

namespace AMG {

class Terrain: public Object {
private:
	const static float SIZE;			/**< Terrain size */
	const static int VERTEX_COUNT;		/**< Number of vertices per terrain unit */
public:
	static Shader *terrainShader;		/**< Shader used for Terrain rendering */
	Terrain(float x, float y, const char *texture);
	void draw(Renderer *renderer);
	virtual ~Terrain();
};

}

#endif
