#ifndef ENTITYCLOUD_H
#define ENTITYCLOUD_H

#include "mesh.h"
#include "camera.h"
#include "entity.h"
#include "entityMesh.h"


class EntityCloud : public Entity
{


public:
	EntityCloud();
	~EntityCloud();

	void render();
	void render(Camera* cam);
	void Init(Camera* cam);

	

	// model matrix 
	Mesh* plane;
	Material mat;
	float distance;

};


#endif
