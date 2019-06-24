#ifndef ENTITYPLAYER_H
#define ENTITYPLAYER_H

#include "mesh.h"
#include "camera.h"
#include "entity.h"
#include "entityMesh.h"
#include "animation.h"
#include "GUI.h"

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


	void update(float dt, std::vector<EntityMesh> props, std::vector<EntityMesh> bc,std::vector<EntityMesh> b);
	void playerMovement(float dt, std::vector<EntityMesh>props, std::vector<EntityMesh> bc, std::vector<EntityMesh> b);
	void rotateCannon();


	void checkCollision(std::vector<EntityMesh> props, std::vector<EntityMesh> bc, std::vector<EntityMesh> b,Vector3 newpos,float dt);
	void updateItem(Matrix44 r,Vector3 dir);
	void updateCamera(std::vector<EntityMesh>props, std::vector<EntityMesh> b);
	void updateMatrix();
	void animateCharacter();
	void updateAnim(float time);

	void build(std::vector<EntityMesh>vector,mat_types t);
	void grab(std::vector<EntityMesh> vector);
	void throwItem();
	void shoot(float dt);
	
	void setPosition(float x, float y, float z);

	Vector3 getLocalVector(Vector3 v);
	Vector3 current_position;
	Vector3 velocity;

	float speed;
	float pitch,pitchCannon;
	float yaw,yawCannon;
	Matrix44 initialmatrixCannon;
	float * time;
	float hp;

	Skeleton skeleton;
	Mesh* mesh;
	
	Animation* anim;
	Material mat;
	bool isdead = false;
	

	std::vector<Matrix44> bone_matrices;
	Camera * camera;
	bool isanimated = true, iscarrying = false, isoncannon = false;
	int CarryItem,CannonID;
	EntityMesh Cannon;
	GUI scope;
	

};


#endif
