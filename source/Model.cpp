/*
 * Model.cpp
 *
 *  Created on: 13 jun. 2018
 *      Author: Andrés
 */

#include "Model.h"
#include "Debug.h"
#include "Bone.h"

#include <stdio.h>
#include <stdlib.h>

namespace AMG {

Model::Model(const char *path, Shader *shader) {
	this->nobjects = 0;
	this->nmaterials = 0;
	this->objects = NULL;
	this->materials = NULL;

	FILE *f = fopen(path, "rb");
	if(f == NULL) Debug::showError(FILE_NOT_FOUND, (void*)path);

	char sign[3];
	fread(sign, sizeof(char), 3, f);
	if(sign[0] != 'A' || sign[1] != 'M' || sign[2] != 'D') Debug::showError(WRONG_SIGNATURE, (void*)path);

	nmaterials = 0;
	fread(&nmaterials, sizeof(unsigned char), 1, f);
	materials = (Material**) calloc (nmaterials, sizeof(Material*));

	for(unsigned int i=0;i<nmaterials;i++){
		float buff[11];
		fread(buff, sizeof(float), 11, f);
		unsigned char len;
		fread(&len, sizeof(unsigned char), 1, f);
		char *path = (char*) calloc (len+1, sizeof(char));
		fread(path, len, sizeof(char), f);
		path[len] = 0;
		materials[i] = new Material(buff, path);
		materials[i]->setDependency(true);
		free(path);
	}

	nobjects = 0;
	fread(&nobjects, sizeof(unsigned char), 1, f);
	objects = (Object**) calloc (nobjects, sizeof(Object*));

	unsigned short nvertices;
	unsigned short nindices;
	unsigned char ngroups;
	unsigned char nbones;

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

		objects[i] = new Object();
		objects[i]->setDependency(true);
		objects[i]->addBuffer(vertices, vertices_size, 3, GL_FLOAT);
		objects[i]->addBuffer(texcoords, texcoords_size, 2, GL_FLOAT);
		objects[i]->addBuffer(normals, vertices_size, 3, GL_FLOAT);
		objects[i]->setIndexBuffer(indices, nindices*sizeof(unsigned short));
		objects[i]->setMaterialGroups(groups, ngroups, materials, nmaterials);

		fread(&nbones, sizeof(unsigned char), 1, f);
		bone_t *bones = (bone_t*) calloc (nbones, sizeof(bone_t));
		for(unsigned char j=0;j<nbones;j++){
			bone_t *bone = &bones[j];
			fread(&bone->parent, sizeof(unsigned short), 1, f);
			fread(&bone->nchildren, sizeof(unsigned short), 1, f);
			//fprintf(stderr, "%u %u ", bone->parent, bone->nchildren);
			if(bone->nchildren > 0){
				bone->children = (unsigned short*) malloc (bone->nchildren * sizeof(unsigned short));
				fread(bone->children, sizeof(unsigned short), bone->nchildren, f);
			}
			fread(bone->localbindmatrix, sizeof(float), 16, f);
			fread(bone->matrix_inv, sizeof(float), 16, f);

			/*for(unsigned int k=0;k<bone->nchildren;k++){
				fprintf(stderr, "{%u}", bone->children[k]);
			}
			fprintf(stderr, "\n");*/
		}
		//fflush(stderr);

		objects[i]->createBoneHierarchy(bones, nbones, shader);

		if(nbones > 0){
			float *weights = (float*) malloc (nvertices*4*sizeof(float));
			fread(weights, sizeof(float), 4*nvertices, f);
			unsigned short *weights_bones = (unsigned short*) malloc (nvertices*4*sizeof(unsigned short));
			fread(weights_bones, sizeof(unsigned short), 4*nvertices, f);
			objects[i]->addBuffer(weights, nvertices*4*sizeof(float), 4, GL_FLOAT);
			objects[i]->addBuffer(weights_bones, nvertices*4*sizeof(unsigned short), 4, GL_UNSIGNED_SHORT);
			/*for(int k=0;k<nvertices;k++){
				fprintf(stderr, "%f %f %f %f | %d %d %d %d\n", weights[(k<<2) + 0], weights[(k<<2) + 1], weights[(k<<2) + 2], weights[(k<<2) + 3],
						weights_bones[(k<<2) + 0], weights_bones[(k<<2) + 1], weights_bones[(k<<2) + 2], weights_bones[(k<<2) + 3]);
			}
			fflush(stderr);*/
			free(weights);
			free(weights_bones);
		}

		free(vertices);
		free(texcoords);
		free(normals);
		free(indices);
		// Grupos no, se eliminan en el objeto
	}

	fclose(f);
}

void Model::draw(Renderer *renderer, Shader *shader){
	for(unsigned int i=0;i<nobjects;i++){
		objects[i]->draw(renderer, shader);
	}
}

Model::~Model() {
	for(unsigned int i=0;i<nobjects;i++){
		delete objects[i];
	}
	for(unsigned int i=0;i<nmaterials;i++){
		delete materials[i];
	}
	if(objects) free(objects);
	if(materials) free(materials);
}

} /* namespace AMG */
