#ifndef ENTITYPLAYER_H
#define ENTITYPLAYER_H

#include "mesh.h"
#include "camera.h"
#include "entity.h"
#include "entityMesh.h"
#include "animation.h"


class EntityPlayer : public Entity
{


public:
	EntityPlayer();
	~EntityPlayer();

	void render();
	void render(Camera* cam);
	void Init(Camera* cam);

	void move(Vector3 delta);
	void rotate(float angle, Vector3 axis);
	Vector3 getLocalVector(Vector3 v);
	Vector3 current_position;
	float current_YRotation;

	Mesh* mesh;
	Animation* anim;
	Material mat;
	std::vector<Matrix44> bone_matrices;
	Camera * camera;
	

};


#endif
