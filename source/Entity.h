/**
 * @file Entity.h
 * @brief File to declare Entities
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
	AMG_AUDIO = 4,
};

/**
 * @class Entity
 * @brief An engine entity
 */
class Entity {
public:
	static int nEntities;						/**< Number of entities */
	Entity();
	static char *getFullPath(const char *path, int type);
	virtual ~Entity();
	static void destroyEntities();
};

}

#endif
