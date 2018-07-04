/**
 * @file Model.h
 * @brief Describes model loading and rendering
 */

// Includes C/C++
#include <stdio.h>
#include <stdlib.h>

// Own includes
#include "Model.h"
#include "Debug.h"
#include "Bone.h"

namespace AMG {

/**
 * @brief Constructor for a 3D Model
 * @param path Path for the *.amd file
 * @param shader Shader to be used
 */
Model::Model(const char *path, Shader *shader) {

	// Initialise variables
	this->nobjects = 0;
	this->nmaterials = 0;
	this->nanimations = 0;
	this->objects = NULL;
	this->materials = NULL;
	this->animations = NULL;
	this->fps = 0;
	this->shader = shader;
	shader->enable();

	// Open file
	FILE *f = fopen(path, "rb");
	if(f == NULL) Debug::showError(FILE_NOT_FOUND, (void*)path);

	// Check signature
	char sign[3];
	fread(sign, sizeof(char), 3, f);
	if(sign[0] != 'A' || sign[1] != 'M' || sign[2] != 'D') Debug::showError(WRONG_SIGNATURE, (void*)path);

	// Set up material information
	nmaterials = 0;
	fread(&nmaterials, sizeof(unsigned char), 1, f);
	materials = (Material**) malloc (nmaterials * sizeof(Material*));

	// Read all materials
	for(unsigned int i=0;i<nmaterials;i++){
		float buff[11];
		fread(buff, sizeof(float), 11, f);
		unsigned char len;
		fread(&len, sizeof(unsigned char), 1, f);
		char *path = NULL;
		if(len > 0){		// Only if we have a texture
			path = (char*) calloc (len+1, sizeof(char));
			fread(path, len, sizeof(char), f);
			path[len] = 0;
		}
		materials[i] = new Material(buff, path);
		materials[i]->setDependency(true);
		free(path);
	}

	// Set up object information
	fread(&nobjects, sizeof(unsigned char), 1, f);
	objects = (Object**) malloc (nobjects * sizeof(Object*));

	// Temporal variables
	unsigned short nvertices;
	unsigned short nindices;
	unsigned char ngroups;
	unsigned char nbones;

	// Read each object
	for(unsigned int i=0;i<nobjects;i++){
		fread(&nvertices, sizeof(unsigned short), 1, f);
		unsigned int vertices_size = nvertices*3*sizeof(float);
		unsigned int texcoords_size = nvertices*2*sizeof(float);
		float *vertices = (float*) malloc (vertices_size);
		float *texcoords = (float*) malloc (texcoords_size);
		float *normals = (float*) malloc (vertices_size);
		fread(vertices, sizeof(float), nvertices*3, f);
		fread(texcoords, sizeof(float), nvertices*2, f);
		fread(normals, sizeof(float), nvertices*3, f);
		fread(&nindices, sizeof(unsigned short), 1, f);
		unsigned short *indices = (unsigned short*) malloc (nindices*sizeof(unsigned short));
		fread(indices, sizeof(unsigned short), nindices, f);
		fread(&ngroups, sizeof(unsigned char), 1, f);
		unsigned short *groups = (unsigned short*) malloc (ngroups*3*sizeof(unsigned short));
		fread(groups, sizeof(unsigned short), 3*ngroups, f);

		// Create a new object
		objects[i] = new Object();
		objects[i]->setDependency(true);
		objects[i]->addBuffer(vertices, vertices_size, 3, GL_FLOAT);
		objects[i]->addBuffer(texcoords, texcoords_size, 2, GL_FLOAT);
		objects[i]->addBuffer(normals, vertices_size, 3, GL_FLOAT);
		objects[i]->setIndexBuffer(indices, nindices*sizeof(unsigned short));
		objects[i]->setMaterialGroups(groups, ngroups, materials, nmaterials);

		// Read bone information
		fread(&nbones, sizeof(unsigned char), 1, f);
		bone_t *bones = (bone_t*) calloc (nbones, sizeof(bone_t));

		// Read each bone
		for(unsigned char j=0;j<nbones;j++){
			bone_t *bone = &bones[j];
			fread(&bone->parent, sizeof(unsigned short), 1, f);
			fread(&bone->nchildren, sizeof(unsigned short), 1, f);
			if(bone->nchildren > 0){
				bone->children = (unsigned short*) malloc (bone->nchildren * sizeof(unsigned short));
				fread(bone->children, sizeof(unsigned short), bone->nchildren, f);
			}
			fread(bone->localbindmatrix, sizeof(float), 16, f);
			fread(bone->matrix_inv, sizeof(float), 16, f);
		}

		// Create our bone structure
		objects[i]->createBoneHierarchy(bones, nbones);

		// Load up the weights buffer
		if(nbones > 0){
			float *weights = (float*) malloc (nvertices*4*sizeof(float));
			fread(weights, sizeof(float), 4*nvertices, f);
			unsigned short *weights_bones = (unsigned short*) malloc (nvertices*4*sizeof(unsigned short));
			fread(weights_bones, sizeof(unsigned short), 4*nvertices, f);
			objects[i]->addBuffer(weights, nvertices*4*sizeof(float), 4, GL_FLOAT);
			objects[i]->addBuffer(weights_bones, nvertices*4*sizeof(unsigned short), 4, GL_UNSIGNED_SHORT);
			free(weights);
			free(weights_bones);
		}

		// Read animation data
		fread(&this->nanimations, sizeof(unsigned char), 1, f);
		fread(&this->fps, sizeof(unsigned char), 1, f);

		// Prepare temporary buffers
		float *data = (float*) malloc (7*nbones*sizeof(float));
		float instant;

		if(this->nanimations > 0){
			this->animations = (Animation**) calloc (this->nanimations, sizeof(Animation*));
			for(unsigned int j=0;j<this->nanimations;j++){
				unsigned int nkeyframes = 0;
				fread(&nkeyframes, sizeof(unsigned short), 1, f);
				Keyframe **keyframes = (Keyframe**) calloc (nkeyframes, sizeof(Keyframe*));
				for(unsigned int i=0;i<nkeyframes;i++){
					fread(&instant, sizeof(float), 1, f);
					fread(data, sizeof(float), 7*nbones, f);
					keyframes[i] = new Keyframe(instant, data, nbones);
				}
				this->animations[j] = new Animation(keyframes, nkeyframes);
				this->animations[j]->setDependency(true);
			}
		}

		// Free temporary data
		free(vertices);
		free(texcoords);
		free(normals);
		free(indices);
		free(data);
		// Groups are deleted in its Object
	}

	// Close file
	fclose(f);
}

/**
 * @brief Draw a 3D model previously loaded
 */
void Model::draw(){
	shader->enable();
	Renderer::currentRenderer->updateLighting();
	Renderer::currentRenderer->updateFog();
	for(unsigned int i=0;i<nobjects;i++){
		objects[i]->draw();
	}
}

/**
 * @brief Animate an Object in this Model
 * @param objIndex Object to animate
 * @param animIndex Which animation to apply to the object
 */
void Model::animate(unsigned int objIndex, unsigned int animIndex){
	if(objIndex < nobjects && animIndex < nanimations){
		animations[animIndex]->increaseTime(fps * Renderer::currentRenderer->getDelta());
		Keyframe *first, *last;
		float progress = animations[animIndex]->getKeyframes(&first, &last);
		animations[animIndex]->animateBone(objects[objIndex]->rootBone, first, last, progress);
	}
}

/**
 * @brief Destructor of a 3D model
 */
Model::~Model() {
	for(unsigned int i=0;i<nobjects;i++){
		delete objects[i];
	}
	for(unsigned int i=0;i<nmaterials;i++){
		delete materials[i];
	}
	for(unsigned int i=0;i<nanimations;i++){
		delete animations[i];
	}
	if(objects) free(objects);
	if(materials) free(materials);
	if(animations) free(animations);
}

}
