/**
 * @file Object.h
 * @brief Describe a 3D object
 */

#ifndef OBJECT_H_
#define OBJECT_H_

// Includes OpenGL
#include <glm/glm.hpp>
using namespace glm;

// Own includes
#include "MeshData.h"
#include "Renderer.h"
#include "Material.h"
#include "Bone.h"

namespace AMG {

/**
 * @class Object
 * @brief Describes a 3D object, extends MeshData
 */
class Object: public MeshData {
private:
	unsigned short *groups;			/**< Buffer of material groups, each entry has a material index */
	unsigned int ngroups;			/**< Number of groups in this object */
public:
	Material **materials;			/**< Buffer of materials, same for a Model */
	unsigned int nmaterials;		/**< Number of materials, same for a Model */
	Bone *rootBone;					/**< Bone hierarchy, NULL if there are no bones */
	vec3 position;					/**< Object position */
	quat rotation;					/**< Object rotation */
	vec3 scale;						/**< Object scale */
	Object();
	void setMaterialGroups(unsigned short *groups, unsigned int ngroups, Material **materials, unsigned int nmaterials);
	void createBoneHierarchy(bone_t *bones, unsigned int nbones);
	void draw();
	virtual ~Object();
};

}

#endif
