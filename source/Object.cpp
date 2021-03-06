/**
 * @file Object.cpp
 * @brief Describe a 3D object
 */

// Includes C/C++
#include <stdio.h>
#include <stdlib.h>

// Own includes
#include "Object.h"
#include "Debug.h"
#include "Renderer.h"

namespace AMG {

/**
 * @brief Constructor for an Object
 */
Object::Object() {
	this->position = vec3(0.0f, 0.0f, 0.0f);
	this->rotation = quat(1, 0, 0, 0);
	this->scale = vec3(1.0f, 1.0f, 1.0f);
	this->groups = NULL;
	this->ngroups = 0;
	this->materials = NULL;		// References from a Model object
	this->nmaterials = 0;
	this->rootBone = NULL;
	this->bbox = vec3(0.0f, 0.0f, 0.0f);
	this->visible = true;
}

/**
 * @brief Set an object's material groups
 * @param groups List of groups in this object
 * @param ngroups Number of groups in this object
 * @param materials Buffer of materials referenced by the groups
 * @param nmaterials Number of materials in the buffer
 */
void Object::setMaterialGroups(unsigned short *groups, unsigned int ngroups, Material **materials, unsigned int nmaterials){
	this->groups = groups;
	this->ngroups = ngroups;
	this->materials = materials;
	this->nmaterials = nmaterials;
}

/**
 * @brief Builds up a recursive bone tree
 * @param bones Bone structure loaded from a *.amd file
 * @param nbones Number of bones in the structure
 */
void Object::createBoneHierarchy(bone_t *bones, unsigned int nbones){
	// Check number of bones is not so big
	if(nbones > 16){
		Debug::showError(TOO_MANY_BONES, NULL);
	}
	// Search root bone (must be only one)
	for(unsigned int i=0;i<nbones;i++){
		if(bones[i].parent == 0xFFFF){
			rootBone = new Bone(i);
			rootBone->createChildren(bones, nbones);
		}
	}
	free(bones);
}

/**
 * @brief Draw an Object
 */
void Object::draw(){

	// Transform the object
	Renderer::setTransformationZ(position, rotation, scale);
	Renderer::updateMVP();
	visible = Renderer::isBBoxVisible(bbox);
	if(!visible) return;

	// Transform each bone
	if(rootBone)
		rootBone->calculateBoneMatrix(NULL);

	this->enableBuffers();

	for(unsigned int i=0;i<ngroups;i++){
		int first = groups[i*3 + 0]*3;
		int last = groups[i*3 + 1]*3;
		int mat_index = groups[i*3 + 2];
		if(groups[i*3 + 2] < nmaterials){		// Valid range of materials
			materials[mat_index]->apply();
			glDrawElements(GL_TRIANGLES, last - first, GL_UNSIGNED_SHORT, (void*)(first << 1));
			materials[mat_index]->disable();
		}
	}

	this->disableBuffers();
}

/**
 * @brief Draw an Object in the simplest way possible
 */
void Object::drawSimple(){

	// Transform the object
	Renderer::setTransformationZ(position, rotation, scale);
	Renderer::updateMVP();
	visible = Renderer::isBBoxVisible(bbox);
	if(!visible) return;

	// Enable buffers
	glBindVertexArray(this->id);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->indexid);
	buffer_info &binfo = info.at(0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, binfo.id);
	glVertexAttribPointer(0, binfo.size, GL_FLOAT, GL_FALSE, 0, (void*)0);

	// Draw elements
	glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_SHORT, NULL);

	// Disable buffers
	glDisableVertexAttribArray(0);
}

/**
 * Destructor for an Object
 */
Object::~Object() {
	if(groups) free(groups);
	if(rootBone) delete rootBone;
}

}
