/**
 * @file Object.h
 * @brief Describe a 3D object
 */

#ifndef OBJECT_H_
#define OBJECT_H_

// Includes OpenGL
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
using namespace glm;

// Own includes
#include "MeshData.h"
#include "Material.h"
#include "Bone.h"

namespace AMG {

/**
 * @class Object
 * @brief Describes a 3D object, extends MeshData
 */
class Object: public MeshData {
private:
	Bone *rootBone;					/**< Bone hierarchy, NULL if there are no bones */
	vec3 bbox;						/**< Bounding box, without transformations */
protected:
	Material **materials;			/**< Buffer of materials, same for a Model */
	unsigned int nmaterials;		/**< Number of materials, same for a Model */
	unsigned short *groups;			/**< Buffer of material groups, each entry has a material index */
	unsigned int ngroups;			/**< Number of groups in this object */
	vec3 position;					/**< Object position */
	quat rotation;					/**< Object rotation */
	vec3 scale;						/**< Object scale */
public:
	unsigned int getNMaterials(){ return nmaterials; }
	Material *getMaterial(int i){ return materials[i]; }
	Bone *getRootBone(){ return rootBone; }
	vec3 &getPosition(){ return position; }
	quat &getRotation(){ return rotation; }
	vec3 &getScale(){ return scale; }
	vec3 &getBBox(){ return bbox; }

	Object();
	void setMaterialGroups(unsigned short *groups, unsigned int ngroups, Material **materials, unsigned int nmaterials);
	void createBoneHierarchy(bone_t *bones, unsigned int nbones);
	void draw();
	virtual ~Object();
};

}

#endif
