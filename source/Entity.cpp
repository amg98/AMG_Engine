/**
 * @file Entity.cpp
 * @brief File to declare Entity features, used in trash collector
 */

// Includes C/C++
#include <stdio.h>

// Own includes
#include "Entity.h"

namespace AMG {

static char _fullpath[256];
int Entity::nEntities = 0;

/**
 * @brief Get the full path of an Entity
 * @param path The short path
 * @param type Type of entity we are referring, see EntityTypes
 * @return The full path for this Entity
 */
char *Entity::getFullPath(const char *path, int type){
	switch(type){
		case AMG_MODEL:
			sprintf(_fullpath, "Data/Model/%s", path);
			break;
		case AMG_FONT:
			sprintf(_fullpath, "Data/Font/%s", path);
			break;
		case AMG_TEXTURE:
			sprintf(_fullpath, "Data/Texture/%s", path);
			break;
		case AMG_SHADER:
			sprintf(_fullpath, "Data/Shader/%s", path);
			break;
		case AMG_AUDIO:
			sprintf(_fullpath, "Data/Audio/%s", path);
			break;
		default: break;
	}
	return _fullpath;
}

/**
 * @brief Constructor of an Entity
 */
Entity::Entity() {
	nEntities ++;
}

/**
 * @brief Destructor of an entity
 */
Entity::~Entity() {
	nEntities --;
}

}
