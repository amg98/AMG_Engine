/**
 * @file Terrain.cpp
 * @brief Description of terrain objects
 */

// Includes C/C++
#include <stdlib.h>

// Own includes
#include "Renderer.h"
#include "Terrain.h"

namespace AMG {

// Static variables
const float Terrain::SIZE = 128;
const int Terrain::VERTEX_COUNT = 128;

/**
 * @brief Constructor for a Terrain
 * @param texture Texture to use for this Terrain
 */
Terrain::Terrain(float x, float y, const char *texture){

	// Set position
	this->position.x = x * SIZE;
	this->position.z = y * SIZE;

	// Load up vertices and stuff
	// From ThinMatrix: https://www.dropbox.com/s/47qk4yrz5v9lb61/Terrain%20Generation%20Code.txt?dl=0
	int count = VERTEX_COUNT * VERTEX_COUNT;
	float *vertices = (float*) malloc (count * 3 * sizeof(float));
	float *normals = (float*) malloc (count * 3 * sizeof(float));
	float *textureCoords = (float*) malloc (count * 2 * sizeof(float));
	unsigned short *indices = (unsigned short*) malloc (6*(VERTEX_COUNT-1)*(VERTEX_COUNT-1) * sizeof(unsigned short));
	int vertexPointer = 0;
	for(int i=0;i<VERTEX_COUNT;i++){
		for(int j=0;j<VERTEX_COUNT;j++){
			vertices[vertexPointer*3] = (float)j/((float)VERTEX_COUNT - 1) * SIZE;
			vertices[vertexPointer*3+1] = 0;
			vertices[vertexPointer*3+2] = (float)i/((float)VERTEX_COUNT - 1) * SIZE;
			normals[vertexPointer*3] = 0;
			normals[vertexPointer*3+1] = 1;
			normals[vertexPointer*3+2] = 0;
			textureCoords[vertexPointer*2] = (float)j/((float)VERTEX_COUNT - 1);
			textureCoords[vertexPointer*2+1] = (float)i/((float)VERTEX_COUNT - 1);
			vertexPointer++;
		}
	}
	int pointer = 0;
	for(int gz=0;gz<VERTEX_COUNT-1;gz++){
		for(int gx=0;gx<VERTEX_COUNT-1;gx++){
			int topLeft = (gz*VERTEX_COUNT)+gx;
			int topRight = topLeft + 1;
			int bottomLeft = ((gz+1)*VERTEX_COUNT)+gx;
			int bottomRight = bottomLeft + 1;
			indices[pointer++] = topLeft;
			indices[pointer++] = bottomLeft;
			indices[pointer++] = topRight;
			indices[pointer++] = topRight;
			indices[pointer++] = bottomLeft;
			indices[pointer++] = bottomRight;
		}
	}

	addBuffer(vertices, count * 3 * sizeof(float), 3, GL_FLOAT);
	addBuffer(textureCoords, count * 2 * sizeof(float), 2, GL_FLOAT);
	addBuffer(normals, count * 3 * sizeof(float), 3, GL_FLOAT);
	setIndexBuffer(indices, 6*(VERTEX_COUNT-1)*(VERTEX_COUNT-1) * sizeof(unsigned short));
	this->vertices = NULL;
	this->nvertices = 0;
	free(vertices);
	free(normals);
	free(textureCoords);
	free(indices);

	// Create the material
	Material **mat = (Material**) malloc (sizeof(Material*));
	mat[0] = new Material(texture);
	unsigned short *groups = (unsigned short*) malloc (3 * sizeof(unsigned short));
	groups[0] = 0;
	groups[1] = 2*(VERTEX_COUNT-1)*(VERTEX_COUNT-1)-1;
	groups[2] = 0;
	setMaterialGroups(groups, 1, mat, 1);
}

/**
 * @brief Render a Terrain object
 * @param renderer Window to draw this terrain
 */
void Terrain::draw(){
	Renderer::updateFog();
	Renderer::updateLighting();
	Renderer::setTransformation(position, rotation, scale);
	Renderer::updateMVP();

	this->enableBuffers();

	int first = groups[0]*3;
	int last = groups[1]*3;
	if(groups[2] < nmaterials){		// Valid range of materials
		materials[groups[2]]->apply();
		glDrawElements(GL_TRIANGLES, last - first, GL_UNSIGNED_SHORT, (void*)(first << 1));
		materials[groups[2]]->disable();
	}

	this->disableBuffers();
}

/**
 * @brief Destructor for a Terrain
 */
Terrain::~Terrain() {
	if(materials){
		if(materials[0]) delete materials[0];
		free(materials);
	}
}

}
