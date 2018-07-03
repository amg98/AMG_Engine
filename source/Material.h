/**
 * @file Material.h
 * @brief Defines materials
 */

#ifndef MATERIAL_H_
#define MATERIAL_H_

// Includes C/C++
#include <vector>

// Includes OpenGL
#include <glm/glm.hpp>
using namespace glm;

// Own includes
#include "Entity.h"
#include "Texture.h"

namespace AMG {

/**
 * @class Material
 * @brief Class which holds a material
 */
class Material: public Entity {
private:
	vec4 diffuse;				/**< Material diffuse color */
	vec4 specular;				/**< Material specular color */
	vec4 ambient;				/**< Material ambient color */
	float diffusePower;			/**< Material diffuse power */
	float specularPower;		/**< Material specular power */
	std::vector<Texture*> textures;	/**< Material textures */
public:
	Material(const char *path);
	Material(float *data, const char *path);
	void addTexture(const char *texture);
	void apply();
	virtual ~Material();
};

}

#endif
