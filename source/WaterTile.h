/**
 * @file WaterTile.h
 * @brief Realistic water effects
 */

#ifndef WATERTILE_H_
#define WATERTILE_H_

// Includes OpenGL
#include <GL/glew.h>
#include <glm/glm.hpp>
using namespace glm;

// Own includes
#include "Framebuffer.h"

namespace AMG {

/**< Function callback to draw in the water engine */
typedef void (*AMG_WaterFunctionCallback)(vec4);

// Defines
#define AMG_WATER_CLIPPING_PLANE 7	/**< Clipping plane for water rendering */

/**
 * @class WaterTile
 * @brief Rendering of water zones
 */
class WaterTile: public Entity {
private:
	GLuint id;					/**< VAO id */
	GLuint bufId;				/**< Vertex VBO id */
	vec3 position;				/**< Water tile position */
	vec3 scale;					/**< Water tile scale */
	Framebuffer *reflection;	/**< Reflection framebuffer */
	Framebuffer *refraction; 	/**< Refraction framebuffer */
	Texture *dudv;				/**< DUDV texture map */
	Texture *normalMap;			/**< Water normal map */
	float waveSpeed;			/**< Wave speed */
	float moveFactor;			/**< Wave movement */
	float waterSpecular;		/**< Specular component */
	float waterReflectivity;	/**< Water reflectivity */
	vec4 waterColor;			/**< Water color */
	static Shader *waterShader;	/**< Water rendering shader */
public:
	static void initialize();
	static void finish();

	void setWaveSpeed(float speed){ waveSpeed = speed; }
	float getWaveSpeed(){ return waveSpeed; }
	vec4 &getWaterColor(){ return waterColor; }
	float &getWaterSpecular(){ return waterSpecular; }
	float &getWaterReflectivity(){ return waterReflectivity; }
	Texture *getReflectionTexture(){ return reflection->getColorTexture(); }
	Texture *getRefractionTexture(){ return refraction->getColorTexture(); }

	WaterTile(const char *normalMapPath, const char *dudvpath, vec3 pos, float size);
	void prepare(AMG_WaterFunctionCallback render);
	void draw();
	virtual ~WaterTile();
};

}

#endif
