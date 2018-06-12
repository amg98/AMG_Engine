/*
 * Entity.cpp
 *
 *  Created on: 12 jun. 2018
 *      Author: Andr�s
 */

#include "Entity.h"
#include <iostream>

namespace AMG {

std::vector<Entity*> Entity::entities;

Entity::Entity() {
	entities.push_back(this);
	//std::cout << "Size: " << entities.size() << std::endl;
}

Entity::~Entity() {
	for(unsigned int i=0;i<entities.size();i++){
		if(entities.at(i) == this){
			entities.erase(entities.begin()+i);
			//std::cout << "Element " << i << " erased, new size " << entities.size() << std::endl;
			i = entities.size();
		}
	}
}

void Entity::destroyEntities(){
	for(unsigned int i=entities.size()-1;i>=0;i--){
		delete entities[i];
	}
	entities.clear();
}

} /* namespace AMG */
