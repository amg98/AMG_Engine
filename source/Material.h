/*
 * Material.h
 *
 *  Created on: 13 jun. 2018
 *      Author: Andrés
 */

#ifndef MATERIAL_H_
#define MATERIAL_H_

#include "Entity.h"
#include "Texture.h"
#include "Shader.h"

#include <glm/glm.hpp>
using namespace glm;

namespace AMG {

class Material: public Entity {
private:
	vec4 diffuse;
	vec4 specular;
	vec4 ambient;
	float diffusePower;
	float specularPower;
	Texture *texture;
public:
	Material(float *data, const char *path);
	void apply(Shader *shader);
	virtual ~Material();
};

} /* namespace AMG */

#endif /* MATERIAL_H_ */
