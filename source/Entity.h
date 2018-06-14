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
	bool dependant;		// Si ese objeto esta dentro de otro (entonces el objeto padre debe borrarlo)
public:
	Entity();
	void setDependency(bool dependency);
	virtual ~Entity();
	static void destroyEntities();
};

} /* namespace AMG */

#endif /* ENTITY_H_ */
