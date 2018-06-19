/**
 * @file Material.cpp
 * @brief Defines materials
 */

// Includes C/C++
#include <stdio.h>
#include <string.h>

// Own includes
#include "Material.h"

namespace AMG {

/**
 * @brief Constructor for a Material
 * @param data Float data which was loaded from a model
 * @param texture Texture path (if its NULL it doesn't load a texture)
 */
Material::Material(float *data, const char *texture) {
	this->diffuse = vec4(data[0], data[1], data[2], data[4]);
	this->specular = vec4(data[5], data[6], data[7], data[9]);
	this->ambient = vec4(data[10], data[10], data[10], 1.0f);
	this->diffusePower = data[3];
	this->specularPower = data[8];
	this->texture = NULL;

	if(texture){
		char path[128];
		sprintf(path, "Data/Texture/%s", texture);
		this->texture = new Texture(path);
		this->texture->setDependency(true);
	}
}

/**
 * @brief Update material information in a shader
 */
void Material::apply(){
	this->texture->enable();
	// TODO lighting
}

/**
 * @brief Destructor of a Material
 */
Material::~Material() {
	if(this->texture != NULL)
		delete this->texture;
}

}
