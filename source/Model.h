/**
 * @file Model.h
 * @brief Describes model loading and rendering
 */

#ifndef MODEL_H_
#define MODEL_H_

// Own includes
#include "Entity.h"
#include "Object.h"
#include "Material.h"
#include "Renderer.h"
#include "Animation.h"

namespace AMG {

/**
 * @class Model
 * @brief Holds a 3D model and a loader for *.amd files
 */
class Model: public Entity {
private:
	unsigned int fps;				/**< Frames per second */
	unsigned int nmaterials;		/**< Number of materials in the model */
	unsigned int nobjects;			/**< Number of objects in the model	*/
	unsigned int nanimations;		/**< Number of animations */
	Material **materials;			/**< List of materials */
	Object **objects;				/**< List of objects */
	Animation **animations;			/**< List of animations */
public:
	unsigned int getNObjects(){ return nobjects; }
	unsigned int getNAnimations(){ return nanimations; }
	Object *getObject(int i){ return objects[i]; }
	Animation *getAnimation(int i){ return animations[i]; }

	Model(const char *path, Shader *shader, bool tangent=false);
	void draw();
	void animate(unsigned int objIndex, unsigned int animIndex);
	virtual ~Model();
};

}

#endif
