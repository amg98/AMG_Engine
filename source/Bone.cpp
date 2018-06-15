/*
 * Bone.cpp
 *
 *  Created on: 14 jun. 2018
 *      Author: Andrés
 */

#include "Bone.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <glm/gtc/type_ptr.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace AMG {

Bone::Bone(int id, int glid) {
	this->id = id;
	this->openglid = glid;
	this->children = std::vector<Bone*>();
	this->localBindMatrix = mat4(1.0f);
	this->modelMatrixInv = mat4(1.0f);
	this->transformMatrix = mat4(1.0f);
	this->angle = 0.0f;
	this->axis = vec3(0.0f, 1.0f, 0.0f);
	/*fprintf(stderr, "%d\n", id);
	fflush(stderr);*/
}

void Bone::createChildren(bone_t *bones, int nbones, Shader *shader){
	this->localBindMatrix = glm::make_mat4(bones[id].localbindmatrix);
	this->modelMatrixInv = glm::make_mat4(bones[id].matrix_inv);
	/*for(int i=0;i<4;i++){
		for(int j=0;j<4;j++){
			fprintf(stderr, "%f, ", this->modelMatrixInv[i][j]);
		}
		fprintf(stderr, "\n");
	}
	fprintf(stderr, "\n");
	fflush(stderr);*/
	if(bones[id].nchildren > 0){
		for(int i=0;i<bones[id].nchildren;i++){
			char str[64];
			sprintf(str, "boneMatrix[%d]", bones[id].children[i]);
			//fprintf(stderr, "%s ", str);
			Bone *bone = new Bone(bones[id].children[i], glGetUniformLocation(shader->getProgram(), str));
			bone->setDependency(true);
			bone->createChildren(bones, nbones, shader);
			this->children.push_back(bone);
		}
		free(bones[id].children);
	}
}

void Bone::calculateBoneMatrix(Shader *shader, Bone *parent){
	if(parent == NULL){
		this->transformMatrix = this->localBindMatrix * glm::rotate(angle, axis);
		//fprintf(stderr, "ID %d\n", id);
	}else{
		this->transformMatrix = parent->transformMatrix * this->localBindMatrix * glm::rotate(angle, axis);
		//fprintf(stderr, "ID %d from parent %d\n", id, parent->id);
	}
	for(unsigned int i=0;i<this->children.size();i++){
		this->children[i]->calculateBoneMatrix(shader, this);
	}
	/*if(id == 1){
		fprintf(stderr, "ID %d\n", this->id);
		for(int i=0;i<4;i++){
			for(int j=0;j<4;j++){
				fprintf(stderr, "%f, ", this->transformMatrix[i][j]);
			}
			fprintf(stderr, "\n");
		}
		fprintf(stderr, "\n");
		for(int i=0;i<4;i++){
					for(int j=0;j<4;j++){
						fprintf(stderr, "%f, ", parent->transformMatrix[i][j]);
					}
					fprintf(stderr, "\n");
				}
				fprintf(stderr, "\n");
				for(int i=0;i<4;i++){
							for(int j=0;j<4;j++){
								fprintf(stderr, "%f, ", this->localBindMatrix[i][j]);
							}
							fprintf(stderr, "\n");
						}
						fprintf(stderr, "\n");
	}*/
	this->transformMatrix *= this->modelMatrixInv;
	glUniformMatrix4fv(this->openglid, 1, GL_FALSE, &this->transformMatrix[0][0]);
}

Bone::~Bone() {
	for(unsigned int i=0;i<children.size();i++){
		delete children[i];
	}
	children.clear();
}

} /* namespace AMG */
