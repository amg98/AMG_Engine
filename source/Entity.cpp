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

/**
 * @brief Constructor of an Entity
 */
Entity::Entity() {
	this->dependant = false;
	entities.push_back(this);
	/*fprintf(stderr, "Size: %d\n", entities.size());
	fflush(stderr);*/
}

/**
 * @brief Destructor of an entity
 */
Entity::~Entity() {
	for(unsigned int i=0;i<entities.size();i++){
		if(entities.at(i) == this){
			entities.erase(entities.begin()+i);
			/*fprintf(stderr, "Element %d erased, new size %d\n", i, entities.size());
			fflush(stderr);*/
			i = entities.size();
		}
	}
}

/**
 * @brief Sets this entity's dependency towards another entity
 * @param dependency true if it depends, false otherwise
 */
void Entity::setDependency(bool dependency){
	this->dependant = dependency;
}

/**
 * @brief Static method to destroy all entities
 * @note Called internally by Renderer::exitProcess()
 */
void Entity::destroyEntities(){
	for(unsigned int i=entities.size()-1;i>0;i--){		// Backwards (first delete a model, then the window)
		if(!entities[i]->dependant)
			delete entities[i];
	}
	entities.clear();
}

} /* namespace AMG */
