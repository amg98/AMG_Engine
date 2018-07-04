/**
 * @file Skybox.cpp
 * @brief Environment mapping cubes utilities
 */

// Own includes
#include "Skybox.h"
#include "Texture.h"

namespace AMG {

#define SIZE 500.0f

static float skyboxData[] = {
		-SIZE,  SIZE, -SIZE,
	   -SIZE, -SIZE, -SIZE,
	    SIZE, -SIZE, -SIZE,
	    SIZE, -SIZE, -SIZE,
	    SIZE,  SIZE, -SIZE,
	   -SIZE,  SIZE, -SIZE,

	   -SIZE, -SIZE,  SIZE,
	   -SIZE, -SIZE, -SIZE,
	   -SIZE,  SIZE, -SIZE,
	   -SIZE,  SIZE, -SIZE,
	   -SIZE,  SIZE,  SIZE,
	   -SIZE, -SIZE,  SIZE,

	    SIZE, -SIZE, -SIZE,
	    SIZE, -SIZE,  SIZE,
	    SIZE,  SIZE,  SIZE,
	    SIZE,  SIZE,  SIZE,
	    SIZE,  SIZE, -SIZE,
	    SIZE, -SIZE, -SIZE,

	   -SIZE, -SIZE,  SIZE,
	   -SIZE,  SIZE,  SIZE,
	    SIZE,  SIZE,  SIZE,
	    SIZE,  SIZE,  SIZE,
	    SIZE, -SIZE,  SIZE,
	   -SIZE, -SIZE,  SIZE,

	   -SIZE,  SIZE, -SIZE,
	    SIZE,  SIZE, -SIZE,
	    SIZE,  SIZE,  SIZE,
	    SIZE,  SIZE,  SIZE,
	   -SIZE,  SIZE,  SIZE,
	   -SIZE,  SIZE, -SIZE,

	   -SIZE, -SIZE, -SIZE,
	   -SIZE, -SIZE,  SIZE,
	    SIZE, -SIZE, -SIZE,
	    SIZE, -SIZE, -SIZE,
	   -SIZE, -SIZE,  SIZE,
	    SIZE, -SIZE,  SIZE
};

const static char skyboxFiles[][16] = {
	"right\0", "left\0", "top\0", "bottom\0", "back\0", "front\0"
};

/**
 * @brief Constructor for a Skybox
 * @param dir Folder where the cube map is
 * @param shader Shader to use for this Skybox
 * @note Each texture must have the same number of mipmaps (only 1 is recommended)
 */
Skybox::Skybox(const char *dir, Shader *shader) {

	// Set the shader
	this->shader = shader;

	// Bind the vertices buffer and enable raw drawing for this object
	addBuffer(skyboxData, sizeof(skyboxData), 3, GL_FLOAT, true);

	// Create the material
	materials = (Material**) malloc (sizeof(Material*));
	char **names = (char**) malloc (AMG_CUBE_SIDES * sizeof(char*));
	for(int i=0;i<AMG_CUBE_SIDES;i++){
		names[i] = (char*) malloc (128 * sizeof(char));
		sprintf(names[i], "%s/%s.dds", dir, skyboxFiles[i]);
	}
	materials[0] = new Material((const char**)names);
	materials[0]->setDependency(true);
	for(int i=0;i<AMG_CUBE_SIDES;i++){
		free(names[i]);
	}
	free(names);
}

void Skybox::draw(){
	shader->enable();		// Skyboxes don't use lighting nor fog
	Renderer::currentRenderer->setTransformation(position, rotation, scale);
	Renderer::currentRenderer->updateMVP();
	materials[0]->apply();
	MeshData::drawRaw();
}

Skybox::~Skybox() {
	if(materials){
		delete materials[0];
		free(materials);
	}
}

}
