/*
 * Object.h
 *
 *  Created on: 13 jun. 2018
 *      Author: Andrés
 */

#ifndef OBJECT_H_
#define OBJECT_H_

#include "MeshData.h"
#include "Renderer.h"
#include "Shader.h"
#include "Material.h"

#include <glm/glm.hpp>
using namespace glm;

namespace AMG {

class Object: public MeshData {
private:
	unsigned short *groups;
	unsigned int ngroups;
	Material **materials;
	unsigned int nmaterials;
public:
	vec3 position;
	float angle;
	vec3 axis;
	vec3 scale;
	Object();
	void setMaterialGroups(unsigned short *groups, unsigned int ngroups, Material **materials, unsigned int nmaterials);
	void draw(Renderer *renderer, Shader *shader);
	virtual ~Object();
};

} /* namespace AMG */

#endif /* OBJECT_H_ */
