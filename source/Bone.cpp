/** @file Bone.cpp
  * @brief Bone hierachy used in skeletal animation
  *
  * Bones are stored recursively, and its root bone always belongs to an Object
  */

// Includes C/C++
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Includes OpenGL
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Own includes
#include "Bone.h"
#include "Renderer.h"

namespace AMG {

/**
 * @brief Constructor of one bone
 * @param id Bone ID to set
 * @param glid ID of the matrix uniform in the shader, return value of glGetUniformLocation()
 */
Bone::Bone(int id, int glid) {
	this->id = id;
	this->openglid = glid + id;				// At the specified array index
	this->children = std::vector<Bone*>();
	this->localBindMatrix = mat4(1.0f);
	this->modelMatrixInv = mat4(1.0f);
	this->transformMatrix = mat4(1.0f);
	this->currentBindMatrix = mat4(1.0f);
}

/**
 * Creates a child bone hierachy from a bone_t structure (which was read from a model file)
 * @param bones The bone structure to be read
 * @param nbones Number of bones in the structure
 */
void Bone::createChildren(bone_t *bones, int nbones){

	// Create needed matrices
	this->localBindMatrix = glm::make_mat4(bones[id].localbindmatrix);
	this->currentBindMatrix = this->localBindMatrix;
	this->modelMatrixInv = glm::make_mat4(bones[id].matrix_inv);

	// If there are children
	if(bones[id].nchildren > 0){
		for(int i=0;i<bones[id].nchildren;i++){
			Bone *bone = new Bone(bones[id].children[i], openglid - id);
			bone->createChildren(bones, nbones);		// Create children recursively
			this->children.push_back(bone);
		}
		free(bones[id].children);		// Free data
	}
}

/**
 * @brief Calculate this bone matrix and update it to the shader
 * @param parent Bone parent, his bone matrix is needed, can be NULL if there is no parent
 */
void Bone::calculateBoneMatrix(Bone *parent){

	// Compute bone matrix
	if(parent == NULL){
		this->transformMatrix = this->currentBindMatrix;
	}else{
		this->transformMatrix = parent->transformMatrix * this->currentBindMatrix;
	}

	// Do it for each child
	for(unsigned int i=0;i<this->children.size();i++){
		children[i]->calculateBoneMatrix(this);
	}

	// Perform the final matrix and push it to the shader
	this->transformMatrix *= this->modelMatrixInv;
	glUniformMatrix4fv(this->openglid, 1, GL_FALSE, &this->transformMatrix[0][0]);
}

/**
 * @brief Destructor of a bone
 */
Bone::~Bone() {
	for(unsigned int i=0;i<children.size();i++){
		delete children[i];
	}
}

}
