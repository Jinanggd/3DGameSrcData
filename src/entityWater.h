#ifndef ENTITYWATER_H
#define ENTITYWATER_H

#include "mesh.h"
#include "camera.h"
#include "entity.h"
#include "entityMesh.h"


class EntityWater : public Entity
{


public:
	EntityWater();
	~EntityWater();

	void render();

	// model matrix 
	Mesh* plane;
	Material mat;


};


#endif
