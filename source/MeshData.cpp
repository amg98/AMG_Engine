/**
 * @file MeshData.h
 * @brief File which contains definitions of mesh data
 */

// Own includes
#include "MeshData.h"

namespace AMG {

/**
 * @brief Constructor for a MeshData object
 */
MeshData::MeshData() {
	glGenVertexArrays(1, &this->id);
	glBindVertexArray(this->id);
	glGenBuffers(1, &this->indexid);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->indexid);
	info = std::vector<buffer_info>();
	this->count = 0;
}

/**
 * @brief Add a buffer to our MeshData
 * @param data Pointer to data
 * @param size Buffer size, in bytes
 * @param comps Number of components per buffer element
 * @param type Type of buffer (GL_FLOAT, ...)
 */
void MeshData::addBuffer(void *data, int size, int comps, GLuint type){
	glBindVertexArray(this->id);
	GLuint bufId;
	glGenBuffers(1, &bufId);
	glBindBuffer(GL_ARRAY_BUFFER, bufId);
	glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
	info.push_back((buffer_info){bufId, comps, type});
}

/**
 * @brief Sets information for the index buffer
 * @param data Pointer to the index buffer
 * @param size Index buffer size, in bytes
 */
void MeshData::setIndexBuffer(void *data, int size){
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->indexid);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
	this->count = size / sizeof(short);
}

/**
 * @brief Draws a mesh
 */
void MeshData::draw(){
	this->enableBuffers();

	glDrawElements(GL_TRIANGLES, this->count, GL_UNSIGNED_SHORT, (void*)0);

	this->disableBuffers();
}

/**
 * @brief Enables buffers to this OpenGL context
 * @note Called internally when drawing a mesh
 */
void MeshData::enableBuffers(){
	glBindVertexArray(this->id);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->indexid);
	for(unsigned int i=0;i<info.size();i++){
		buffer_info binfo = info.at(i);
		glEnableVertexAttribArray(i);
		glBindBuffer(GL_ARRAY_BUFFER, binfo.id);
		if(binfo.type == GL_FLOAT){
			glVertexAttribPointer(i, binfo.size, GL_FLOAT, GL_FALSE, 0, (void*)0);
		}else{
			glVertexAttribIPointer(i, binfo.size, GL_UNSIGNED_SHORT, 0, (void*)0);
		}
	}
}

/**
 * @brief Disables buffers to this OpenGL context
 * @note Called internally when drawing a mesh
 */
void MeshData::disableBuffers(){
	for(unsigned int i=0;i<info.size();i++){
		glDisableVertexAttribArray(i);
	}
}

/**
 * @brief Destructor of a MeshData object
 */
MeshData::~MeshData() {
	for(unsigned int i=0;i<info.size();i++){
		glDeleteBuffers(1, &info.at(i).id);
	}
	glDeleteBuffers(1, &this->indexid);
	glDeleteVertexArrays(1, &this->id);
	info.clear();
}

} /* namespace AMG */
