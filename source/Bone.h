/*
 * Bone.h
 *
 *  Created on: 14 jun. 2018
 *      Author: Andrés
 */

#ifndef BONE_H_
#define BONE_H_

#include "Entity.h"
#include "Shader.h"

#include <vector>

#define GLEW_STATIC
#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
using namespace glm;

namespace AMG {

typedef struct{
	unsigned short parent;
	unsigned short nchildren;
	unsigned short *children;
	float localbindmatrix[16];
	float matrix_inv[16];
}bone_t;

class Bone: public Entity {
private:
	mat4 localBindMatrix;
	mat4 modelMatrixInv;
	int openglid;
public:
	int id;
	mat4 transformMatrix;
	float angle;
	vec3 axis;
	std::vector<Bone*> children;
	Bone(int id, int glid);
	void createChildren(bone_t *bones, int nbones, Shader *shader);
	void calculateBoneMatrix(Shader *shader, Bone *parent);
	virtual ~Bone();
};

} /* namespace AMG */

#endif /* BONE_H_ */
