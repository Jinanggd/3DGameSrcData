#ifndef ENTITYPLAYER_H
#define ENTITYPLAYER_H

#include "mesh.h"
#include "camera.h"
#include "entity.h"
#include "entityMesh.h"
#include "animation.h"

enum type {

	PLAYER,
	TITAN

};


class EntityPlayer : public Entity
{


public:
	EntityPlayer();
	EntityPlayer(float * time);
	EntityPlayer(type);
	~EntityPlayer();

<<<<<<< HEAD
=======
	enum direction {
		KEY_UP,
		KEY_DOWN,
		KEY_RIGHT,
		KEY_LEFT,
	}direction;



>>>>>>> master
	void render(float time);
	void render(Camera* cam);
	void Init(Camera* cam);

	void update(float dt);
	void updateCamera();
	void updateMatrix();
	void animateCharacter();
<<<<<<< HEAD
=======
	void updateAnim(float time);
>>>>>>> master

	void setPosition(float x, float y, float z);

	Vector3 getLocalVector(Vector3 v);
	Vector3 current_position;
<<<<<<< HEAD
	float pitch;
	float yaw;
	float speed;

	enum direction {
		KEY_UP,
		KEY_DOWN,
		KEY_RIGHT,
		KEY_LEFT,
	}direction;
=======
	Vector3 velocity;

	float speed;
	float pitch;
	float yaw;
	float * time;
>>>>>>> master


	Skeleton skeleton;
	Mesh* mesh;
	Animation* anim;
	Material mat;

	std::vector<Matrix44> bone_matrices;
	Camera * camera;
<<<<<<< HEAD
	bool isanimated = false;
=======
	bool isanimated = true;
>>>>>>> master
	

};


#endif
