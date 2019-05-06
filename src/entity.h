#ifndef ENTITY_H
#define ENTITY_H

#include "mesh.h";
#include "camera.h"

class Entity
{


public:

	Entity();
	virtual ~Entity();
	virtual void render();
	virtual void update(float elapsed_time);

	void addChild(Entity* ent);
	void removeChild(Entity* ent);

	// model matrix 
	Matrix44 model;
	std::string name;
	Entity *parent;
	std::vector<Entity*> children;



};


#endif
