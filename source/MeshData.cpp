/*
 * MeshData.cpp
 *
 *  Created on: 4 jun. 2018
 *      Author: Andrés
 */

#include "MeshData.h"

namespace AMG {

MeshData::MeshData() {
	glGenVertexArrays(1, &this->id);
	glBindVertexArray(this->id);
	glGenBuffers(1, &this->indexid);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->indexid);
	buffer_id = std::vector<GLuint>();
	buffer_size = std::vector<int>();
	this->count = 0;
}

void MeshData::addBuffer(void *data, int size, int comps){
	glBindVertexArray(this->id);
	GLuint bufId;
	glGenBuffers(1, &bufId);
	glBindBuffer(GL_ARRAY_BUFFER, bufId);
	glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
	buffer_id.push_back(bufId);
	buffer_size.push_back(comps);
}

void MeshData::setIndexBuffer(void *data, int size){
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->indexid);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
	this->count = size / sizeof(short);
}

void MeshData::draw(){
	glBindVertexArray(this->id);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->indexid);
	for(unsigned int i=0;i<buffer_id.size();i++){
		glEnableVertexAttribArray(i);
		glBindBuffer(GL_ARRAY_BUFFER, buffer_id.at(i));
		glVertexAttribPointer(i, buffer_size.at(i), GL_FLOAT, GL_FALSE, 0, (void*)0);
	}

	glDrawElements(GL_TRIANGLES, this->count, GL_UNSIGNED_SHORT, (void*)0);

	for(unsigned int i=0;i<buffer_id.size();i++){
		glDisableVertexAttribArray(i);
	}
}

MeshData::~MeshData() {
	for(unsigned int i=0;i<buffer_id.size();i++){
		glDeleteBuffers(1, &buffer_id.at(i));
	}
	glDeleteBuffers(1, &this->indexid);
	glDeleteVertexArrays(1, &this->id);
	buffer_id.clear();
	buffer_size.clear();
}

} /* namespace AMG */
