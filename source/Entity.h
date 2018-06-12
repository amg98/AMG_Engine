/*
 * Entity.h
 *
 *  Created on: 12 jun. 2018
 *      Author: Andrés
 */

#ifndef ENTITY_H_
#define ENTITY_H_

#include <vector>

namespace AMG {

class Entity {
private:
	static std::vector<Entity*> entities;
public:
	Entity();
	virtual ~Entity();
	static void destroyEntities();
};

} /* namespace AMG */

#endif /* ENTITY_H_ */
