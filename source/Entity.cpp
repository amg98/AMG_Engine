/**
 * @file Entity.cpp
 * @brief File to declare Entity features, used in trash collector
 */

// Includes C/C++
#include <stdio.h>

// Own includes
#include "Entity.h"

namespace AMG {

std::vector<Entity*> Entity::entities;

static char _fullpath[256];

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
		default: break;
	}
	return _fullpath;
}

/**
 * @brief Constructor of an Entity
 */
Entity::Entity() {
	this->index = entities.size();
	//fprintf(stderr, "i=%d, Size: %d, ", index, entities.size());
	entities.push_back(this);
	/*fprintf(stderr, "Newsize: %d\n", entities.size());
	fflush(stderr);*/
}

/**
 * @brief Destructor of an entity
 */
Entity::~Entity() {
	setDependency();
}

/**
 * @brief Set an Entity's dependant of other (delete it from the vector)
 */
void Entity::setDependency(){
	if(index != -1){
		for(unsigned int i=index+1;i<entities.size();i++){
			entities[i]->index -= 1;
		}
		entities.erase(entities.begin()+index);
		/*fprintf(stderr, "erased %d, new size %d\n", index, entities.size());
		fflush(stderr);*/
		index = -1;
	}
}

/**
 * @brief Static method to destroy all entities
 * @note Called internally by Renderer::exitProcess()
 */
void Entity::destroyEntities(){
	while(entities.size() > 0){
		delete entities[entities.size() - 1];
	}
	std::vector<Entity*>().swap(entities);
}

}
