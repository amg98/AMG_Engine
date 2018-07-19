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
 * @enum EntityTypes
 * @brief Describes the types of Entities we can find in the engine
 */
enum EntityTypes {
	AMG_FONT = 0,
	AMG_MODEL = 1,
	AMG_TEXTURE = 2,
	AMG_SHADER = 3,
};

/**
 * @class Entity
 * @brief An engine entity
 */
class Entity {
private:
	static std::vector<Entity*> entities;		/**< Vector of registered entities */
protected:
	int index;									/**< Index in the vector */
public:
	Entity();
	void setDependency();
	static char *getFullPath(const char *path, int type);
	static std::vector<Entity*> &getEntities(){ return entities; }
	virtual ~Entity();
	static void destroyEntities();
};

}

#endif
