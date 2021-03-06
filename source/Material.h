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
	std::vector<Texture*> textures;	/**< Material textures vector */
	vec4 diffuse;					/**< Material diffuse color */
	vec4 specular;					/**< Material specular color */
	float ambient;					/**< Material ambient color */
	float diffusePower;				/**< Material diffuse power */
	float specularPower;			/**< Material specular power */
	float reflectivity;				/**< Material reflectivity */
	float refractionIndex;			/**< Refraction index */
public:
	vec4 &getDiffuse(){ return diffuse; }
	vec4 &getSpecular(){ return specular; }
	float &getAmbient(){ return ambient; }
	float &getDiffusePower(){ return diffusePower; }
	float &getSpecularPower(){ return specularPower; }
	float &getReflectivity(){ return reflectivity; }
	float &getRefractionIndex(){ return refractionIndex; }
	Texture *getTexture(int id){ return textures[id]; }

	Material();
	Material(Texture *texture);
	Material(const char *path);
	Material(const char **names);
	Material(float *data);
	void addTexture(const char *texture);
	void apply();
	void disable();
	virtual ~Material();
};

}

#endif
