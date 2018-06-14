/*
 * MeshData.h
 *
 *  Created on: 4 jun. 2018
 *      Author: Andrés
 */

#ifndef MeshData_H_
#define MeshData_H_

#define GLEW_STATIC
#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include <vector>
#include "Entity.h"

namespace AMG {

class MeshData : public Entity {
protected:
	GLuint id;
	GLuint indexid;
	std::vector<GLuint> buffer_id;
	std::vector<int> buffer_size;
	int count;
public:
	MeshData();
	void addBuffer(void *data, int size, int comps);
	void setIndexBuffer(void *data, int size);
	void draw();
	virtual ~MeshData();
};

} /* namespace AMG */

#endif /* MeshData_H_ */
