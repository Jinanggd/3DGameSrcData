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

	struct ActionPlane
	{
		Mesh m;
		Matrix44 model;
		Material mat;
		Vector3* pos;
	};


	void render(float time);
	void render(Camera* cam);
	void Init(Camera* cam);

	void update(float dt, std::vector<EntityMesh> props);
	void checkCollision(std::vector<EntityMesh> props, Vector3 newpos,float dt);
	void updateItem(Matrix44 r,Vector3 dir);
	void updateCamera(std::vector<EntityMesh>props);
	void updateMatrix();
	void updateHPBar();
	void animateCharacter();
	void updateAnim(float time);

	void grab(std::vector<EntityMesh> vector);
	void throwItem();
	
	void setPosition(float x, float y, float z);

	Vector3 getLocalVector(Vector3 v);
	Vector3 current_position;
	Vector3 velocity;

	float speed;
	float pitch;
	float yaw;
	float * time;
	float hp;

	Skeleton skeleton;
	Mesh* mesh;
	
	ActionPlane actionplane;
	ActionPlane hpbar;
	
	Animation* anim;
	Material mat;

	std::vector<Matrix44> bone_matrices;
	Camera * camera;
	bool isanimated = true, iscarrying = false, isoncannon = false;
	int CarryItem;
	EntityMesh Cannon;
	

};


#endif
