/**
 * @file Entity.h
 * @brief File to declare Entity features, used in trash collector
 */

#ifndef ENTITY_H_
#define ENTITY_H_

// Includes C/C++
#include <vector>

namespace AMG {

/**
 * @class Entity
 * @brief An engine entity
 */
class Entity {
private:
	static std::vector<Entity*> entities;		/**< Vector of registered entities */
	bool dependant;								/**< If an entity depends on another the root entity must delete it */
public:
	Entity();
	void setDependency(bool dependency);
	virtual ~Entity();
	static void destroyEntities();
};

}

#endif
