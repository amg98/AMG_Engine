/*
 * Object.cpp
 *
 *  Created on: 13 jun. 2018
 *      Author: Andr�s
 */

#include "Object.h"

#include <stdlib.h>

namespace AMG {

Object::Object() {
	this->position = vec3(0.0f, 0.0f, 0.0f);
	this->angle = 0.0f;
	this->axis = vec3(0.0f, 0.0f, 1.0f);
	this->scale = vec3(1.0f, 1.0f, 1.0f);
	this->groups = NULL;
	this->ngroups = 0;
	this->materials = NULL;		// Referencias, no se eliminan
	this->nmaterials = 0;
}

void Object::setMaterialGroups(unsigned short *groups, unsigned int ngroups, Material **materials, unsigned int nmaterials){
	this->groups = groups;
	this->ngroups = ngroups;
	this->materials = materials;
	this->nmaterials = nmaterials;
}

void Object::draw(Renderer *renderer, Shader *shader){
	renderer->setTransformation(position, angle, axis, scale);
	renderer->updateMVP(shader);

	glBindVertexArray(this->id);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->indexid);
	for(unsigned int i=0;i<buffer_id.size();i++){
		glEnableVertexAttribArray(i);
		glBindBuffer(GL_ARRAY_BUFFER, buffer_id.at(i));
		glVertexAttribPointer(i, buffer_size.at(i), GL_FLOAT, GL_FALSE, 0, (void*)0);
	}

	for(unsigned int i=0;i<ngroups;i++){
		int first = groups[i*3 + 0]*3;
		int last = groups[i*3 + 1]*3;
		int mat_index = groups[i*3 + 2];
		if(groups[i*3 + 2] < nmaterials){		// Rango valido de materiales
			materials[mat_index]->apply(shader);
			glDrawElements(GL_TRIANGLES, last - first, GL_UNSIGNED_SHORT, (void*)(first << 1));
		}
	}

	for(unsigned int i=0;i<buffer_id.size();i++){
		glDisableVertexAttribArray(i);
	}
}

Object::~Object() {
	if(groups) free(groups);
}

} /* namespace AMG */
