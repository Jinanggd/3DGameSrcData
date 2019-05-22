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

	enum direction {
		KEY_UP,
		KEY_DOWN,
		KEY_RIGHT,
		KEY_LEFT,
	}direction;



	void render(float time);
	void render(Camera* cam);
	void Init(Camera* cam);

	void update(float dt, std::vector<EntityMesh> props);
	void checkCollision(std::vector<EntityMesh> props,Vector3 newpos,float dt);
	void updateCamera();
	void updateMatrix();
	void animateCharacter();
	void updateAnim(float time);

	void setPosition(float x, float y, float z);

	Vector3 getLocalVector(Vector3 v);
	Vector3 current_position;
	Vector3 velocity;

	float speed;
	float pitch;
	float yaw;
	float * time;


	Skeleton skeleton;
	Mesh* mesh;
	Animation* anim;
	Material mat;

	std::vector<Matrix44> bone_matrices;
	Camera * camera;
	bool isanimated = true;
	

};


#endif
