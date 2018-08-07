/** @file Bone.h
  * @brief Bone hierachy used in skeletal animation
  *
  * Bones are stored recursively, and its root bone always belongs to an Object
  */

#ifndef BONE_H_
#define BONE_H_

// Includes C/C++
#include <vector>

// Includes OpenGL
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
using namespace glm;

// Own includes
#include "Entity.h"

namespace AMG {

/**
 * @struct bone_t
 * @brief Stores information about a bone, used to fill in the Bone class
 */
typedef struct{
	unsigned short parent;			/**< ID of the parent bone, 0xFFFF if there is no parent */
	unsigned short nchildren;		/**< Number of children of this bone, 0 if there are no children */
	unsigned short *children;		/**< Buffer holding bone's children IDs */
	float localbindmatrix[16];		/**< Local binding matrix of the bone, transforms from parent bone's space to bone space */
	float matrix_inv[16];			/**< Inverse of the Model Space to Bone Space Matrix */
}bone_t;

/**
 * @class Bone
 * @brief Class that holds a bone hierarchy
 */
class Bone: public Entity {
private:
	int id;							/**< ID of this bone, it is object-specific */
	mat4 modelMatrixInv;			/**< Bone Space to Model Space Matrix */
	mat4 transformMatrix;			/**< Final bone matrix, which is passed to the shader */
	mat4 localBindMatrix;			/**< Local binding matrix */
	mat4 currentBindMatrix;			/**< Current local binding matrix */
	std::vector<Bone*> children;	/**< Vector holding this bone's children */
public:
	mat4 &getLocalBindMatrix(){ return localBindMatrix; }
	mat4 &getCurrentBindMatrix(){ return currentBindMatrix; }
	std::vector<Bone*> &getChildren(){ return children; }
	int getID(){ return id;}

	Bone(int id);
	void createChildren(bone_t *bones, int nbones);
	void calculateBoneMatrix(Bone *parent);
	virtual ~Bone();
};

}

#endif
