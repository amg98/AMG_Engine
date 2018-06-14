/*
 * Material.cpp
 *
 *  Created on: 13 jun. 2018
 *      Author: Andrés
 */

#include "Material.h"

#include <stdio.h>
#include <string.h>

namespace AMG {

Material::Material(float *data, const char *texture) {
	this->diffuse = vec4(data[0], data[1], data[2], data[4]);
	this->specular = vec4(data[5], data[6], data[7], data[9]);
	this->ambient = vec4(data[10], data[10], data[10], 1.0f);
	this->diffusePower = data[3];
	this->specularPower = data[8];
	char path[128];
	sprintf(path, "Data/Texture/%s", texture);
	this->texture = new Texture(path);
	this->texture->setDependency(true);
}

void Material::apply(Shader *shader){
	this->texture->enable();
	// TODO lighting
}

Material::~Material() {
	delete this->texture;
}

} /* namespace AMG */
