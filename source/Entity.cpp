/*
 * Entity.cpp
 *
 *  Created on: 12 jun. 2018
 *      Author: Andrés
 */

#include "Entity.h"
#include <stdio.h>

namespace AMG {

std::vector<Entity*> Entity::entities;

Entity::Entity() {
	this->dependant = false;
	entities.push_back(this);
	//fprintf(stderr, "Size: %d\n", entities.size());
	//fflush(stderr);
}

Entity::~Entity() {
	for(unsigned int i=0;i<entities.size();i++){
		if(entities.at(i) == this){
			entities.erase(entities.begin()+i);
			//fprintf(stderr, "Element %d erased, new size %d\n", i, entities.size());
			//fflush(stderr);
			i = entities.size();
		}
	}
}

void Entity::setDependency(bool dependency){
	this->dependant = dependency;
}

void Entity::destroyEntities(){
	for(unsigned int i=entities.size()-1;i>=0;i--){
		if(!entities[i]->dependant)
			delete entities[i];
	}
	entities.clear();
}

} /* namespace AMG */
