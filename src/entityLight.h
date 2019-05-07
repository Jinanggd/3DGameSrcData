#ifndef ENTITYLIGHT_H
#define ENTITYLIGHT_H

#include "mesh.h";
#include "camera.h"
#include "entity.h"


class EntityLight : public Entity
{


public:
	EntityLight();
	~EntityLight();

	EntityLight(float dif,float spec, Vector3 pos);
	void setPosition(Vector3 pos);
	void setColor(Vector3 col);
	Vector3 getPosition();

	// model matrix 
	Vector3 color;
	float diffuse_factor;
	float specular_factor;


};


#endif
