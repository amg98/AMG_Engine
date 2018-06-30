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
 * @param texture Texture path (if its NULL it doesn't load a texture)
 */
Material::Material(const char *texture){
	this->diffuse = vec4(1, 1, 1, 1);
	this->specular = vec4(1, 1, 1, 1);
	this->ambient = vec4(0.2f, 0.2f, 0.2f, 1);
	this->diffusePower = 1.0f;
	this->specularPower = 1.0f;
	loadTexture(texture);
}
/**
 * @brief Constructor for a Material
 * @param data Float data which was loaded from a model
 * @param texture Texture path (if its NULL it doesn't load a texture)
 */
Material::Material(float *data, const char *texture) {
	this->diffuse = vec4(data[0], data[1], data[2], data[4]);
	this->specular = vec4(data[5], data[6], data[7], data[9]);
	this->ambient = vec4(data[10], data[10], data[10], data[4]);
	this->diffusePower = data[3];
	this->specularPower = data[8];
	loadTexture(texture);
}

void Material::loadTexture(const char *texture){
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
	if(diffuse.a < 1.0f){
		glDisable(GL_CULL_FACE);
	}else{
		glEnable(GL_CULL_FACE);
	}
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
