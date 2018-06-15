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

typedef struct{
	GLuint id;
	int size;
	GLuint type;
}buffer_info;

class MeshData : public Entity {
protected:
	GLuint id;
	GLuint indexid;
	std::vector<buffer_info> info;
	int count;
public:
	MeshData();
	void addBuffer(void *data, int size, int comps, GLuint type);
	void setIndexBuffer(void *data, int size);
	void draw();
	virtual ~MeshData();
};

} /* namespace AMG */

#endif /* MeshData_H_ */
