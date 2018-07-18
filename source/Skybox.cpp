/**
 * @file Skybox.cpp
 * @brief Environment mapping cubes utilities
 */

// Includes C/C++
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Own includes
#include "Renderer.h"
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
 * @note Each texture must have the same number of mipmaps (only 1 is recommended)
 */
Skybox::Skybox(const char *dir) {

	// Bind the vertices buffer and enable raw drawing for this object
	addBuffer(skyboxData, sizeof(skyboxData), 3, GL_FLOAT, true);

	// Create the material
	materials = (Material**) calloc (1, sizeof(Material*));
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
	Renderer::currentRenderer->setTransformation(position, rotation, scale);
	Renderer::currentRenderer->updateMVP();
	materials[0]->apply();
	MeshData::drawRaw();
}

Skybox::~Skybox() {
	if(materials){
		if(materials[0]) delete materials[0];
		free(materials);
	}
}

}
