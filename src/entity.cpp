#include "entity.h"


Entity::Entity()
{


}


Entity::~Entity()
{

}



void Entity::render() {



}


void Entity::update(float elapsed_time) {



}


void Entity::addChild(Entity* ent) {

	children.push_back(ent);


};

void Entity::removeChild(Entity* ent) {

	auto it = std::find(children.begin(), children.end(), ent);
	if (it != children.end()) { children.erase(it); }

};





