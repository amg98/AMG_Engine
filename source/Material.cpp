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
 * @param texture Texture to use
 */
Material::Material(Texture *texture){
	this->diffuse = vec4(1, 1, 1, 1);
	this->specular = vec4(1, 1, 1, 1);
	this->ambient = 0.2f;
	this->diffusePower = 1.0f;
	this->specularPower = 1.0f;
	this->textures = std::vector<Texture*>();
	textures.push_back(texture);
}

/**
 * @brief Constructor for a Material
 * @param texture Texture path (if its NULL it doesn't load a texture)
 */
Material::Material(const char *texture){
	this->diffuse = vec4(1, 1, 1, 1);
	this->specular = vec4(1, 1, 1, 1);
	this->ambient = 0.2f;
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
	this->ambient = 0.2f;
	this->diffusePower = 1.0f;
	this->specularPower = 1.0f;
	this->textures = std::vector<Texture*>();
	Texture *tex = new Texture(names, true);
	this->textures.push_back(tex);
}

/**
 * @brief Constructor for a Material
 * @param data Float data which was loaded from a model
 */
Material::Material(float *data) {
	this->diffuse = vec4(data[0], data[1], data[2], data[4]);
	this->specular = vec4(data[5], data[6], data[7], data[9]);
	this->ambient = data[10];
	this->diffusePower = data[3];
	this->specularPower = data[8];
	this->textures = std::vector<Texture*>();
}

/**
 * @brief Add a texture to the texture list
 */
void Material::addTexture(const char *texture){
	Texture *tex = NULL;
	if(texture){
		tex = new Texture(texture, textures.size() < Renderer::getsRGBTextures());
		tex->setLod(-0.4f);			// -0.4 level of detail
		tex->setAniso(4.0f);		// 4x anisotropic filtering
		this->textures.push_back(tex);
	}
}

/**
 * @brief Update material information in a shader
 */
void Material::apply(){
	for(unsigned int i=0;i<textures.size();i++){
		textures[i]->animate();
		textures[i]->bind(i);
	}
	if(diffuse.a < 1.0f){
		glDisable(GL_CULL_FACE);
	}else{
		glEnable(GL_CULL_FACE);
	}
	Shader *shader = Renderer::getCurrentShader();
	shader->setUniform("AMG_MaterialDiffuse", diffuse);
	shader->setUniform("AMG_MaterialSpecular", specular);
	shader->setUniform("AMG_MaterialAmbient", ambient);
	shader->setUniform("AMG_DiffusePower", diffusePower);
	shader->setUniform("AMG_SpecularPower", specularPower);
}

void Material::disable(){
	for(unsigned int i=0;i<textures.size();i++){
		textures[i]->unbind(i);
	}
}

/**
 * @brief Destructor of a Material
 */
Material::~Material() {
	for(unsigned int i=0;i<textures.size();i++){
		delete textures[i];
	}
}

}
