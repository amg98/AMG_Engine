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

namespace AMG {

/**
 * @class Model
 * @brief Holds a 3D model and a loader for *.amd files (internal engine model file format)
 */
class Model: public Entity {
private:
	unsigned int nmaterials;		/**< Number of materials in the model */
	Material **materials;			/**< List of materials */
public:
	unsigned int nobjects;			/**< Number of objects in the model	*/
	Object **objects;				/**< List of objects */

	Model(const char *path);
	void draw(Renderer *renderer);
	virtual ~Model();
};

}

#endif
