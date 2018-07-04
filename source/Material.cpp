/**
 * @file Material.cpp
 * @brief Defines materials
 */

// Includes C/C++
#include <stdio.h>
#include <string.h>

// Own includes
#include "Material.h"
#include "Renderer.h"

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
	this->textures = std::vector<Texture*>();
	addTexture(texture);
}

/**
 * @brief Constructor for a Material, if the Texture is a cube map
 * @param texture Cube map filenames
 */
Material::Material(const char **names){
	this->diffuse = vec4(1, 1, 1, 1);
	this->specular = vec4(1, 1, 1, 1);
	this->ambient = vec4(0.2f, 0.2f, 0.2f, 1);
	this->diffusePower = 1.0f;
	this->specularPower = 1.0f;
	this->textures = std::vector<Texture*>();
	Texture *tex = new Texture(names);
	tex->setDependency(true);
	this->textures.push_back(tex);
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
	this->textures = std::vector<Texture*>();
	addTexture(texture);
}

/**
 * @brief Add a texture to the texture list
 */
void Material::addTexture(const char *texture){
	Texture *tex = NULL;
	if(texture){
		char path[128];
		sprintf(path, "Data/Texture/%s", texture);
		tex = new Texture(path);
		tex->setDependency(true);
		this->textures.push_back(tex);
	}
}

/**
 * @brief Update material information in a shader
 */
void Material::apply(){
	for(unsigned int i=0;i<textures.size();i++){
		textures[i]->enable(i);
	}
	if(diffuse.a < 1.0f){
		glDisable(GL_CULL_FACE);
	}else{
		glEnable(GL_CULL_FACE);
	}
	Shader *shader = Renderer::currentRenderer->currentShader;
	shader->setUniform("AMG_MaterialDiffuse", diffuse);
	shader->setUniform("AMG_MaterialSpecular", specular);
	shader->setUniform("AMG_MaterialAmbient", ambient);
	shader->setUniform("AMG_DiffusePower", diffusePower);
	shader->setUniform("AMG_SpecularPower", specularPower);
}

/**
 * @brief Destructor of a Material
 */
Material::~Material() {
	for(unsigned int i=0;i<textures.size();i++){
		delete textures[i];
	}
	textures.clear();
}

}
