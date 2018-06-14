/*
 * Model.h
 *
 *  Created on: 13 jun. 2018
 *      Author: Andrés
 */

#ifndef MODEL_H_
#define MODEL_H_

#include "Entity.h"
#include "Object.h"
#include "Material.h"
#include "Renderer.h"
#include "Shader.h"

namespace AMG {

class Model: public Entity {
private:
	unsigned int nmaterials;
	Material **materials;
public:
	unsigned int nobjects;
	Object **objects;

	Model(const char *path);
	void draw(Renderer *renderer, Shader *shader);
	virtual ~Model();
};

} /* namespace AMG */

#endif /* MODEL_H_ */
