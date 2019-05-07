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
	ent->parent = this;
	this->children.push_back(ent);

};

void Entity::removeChild(Entity* ent) {

	auto it = std::find(children.begin(), children.end(), ent);
	if (it != children.end()) { children.erase(it); }

}

Matrix44 Entity::getModelMatrix()
{
	if (parent)
		return model * parent->getModelMatrix();
	return model;
}

Entity * Entity::getChildren(std::string n)
{
	for (auto it = children.begin(); it != children.end(); ++it) {
		if ((*it)->name == n) {
			return(*it);
		}
	}
	return NULL;
}






