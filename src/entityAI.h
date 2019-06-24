#ifndef ENTITYAI_H
#define ENTITYAI_H

#include "mesh.h"
#include "camera.h"
#include "entity.h"
#include "entityMesh.h"
#include "animation.h"
#include "entityPlayer.h"
#include "GUI.h"



class EntityAI : public Entity
{




public:
	EntityAI();
	EntityAI(float * time);
	EntityAI(float * time, Matrix44 target);
	EntityAI(float * time, EntityPlayer* p, std::vector<EntityMesh> *b);

	~EntityAI();


	void render(float time);
	void render();

	void Init(Camera* cam);

	void update(float dt, std::vector<EntityMesh> props, std::vector<EntityMesh>b);
	void updatedirection(float dt, std::vector<EntityMesh>props);
	void NPCMovement(float dt, std::vector<EntityMesh>props);

	void updateTarget(EntityPlayer p, std::vector<EntityMesh> build);
	void checkCollision(std::vector<EntityMesh> props, std::vector<EntityMesh> b,Vector3 newpos, float dt);
	void updateItem(Matrix44 r, Vector3 dir);
	void updateCamera(std::vector<EntityMesh>props);
	void updateMatrix();
	void updateHPBar();
	void animateCharacter();
	void updateAnim(float time);
	void substractLife();
	bool isnear();

	void setPosition(float x, float y, float z);

	Vector3 getLocalVector(Vector3 v);
	Vector3 current_position;
	Vector3 velocity, direction;
	Matrix44 target;
	bool istargetplayer;
	bool startedattack = false;

	enum { IDLE, SEARCH, ATTACK , HURT, DEAD} state;


	float speed;
	float pitch, yaw;
	float w = 0.2f;

	float * time;
	float animtime;
	float hp;
	int indexBuildable = -1;
	GUI hpbar;
	Skeleton skeleton;
	Mesh* mesh;


	Animation* anim;
	Material mat;


	std::vector<Matrix44> bone_matrices;
	Camera * camera;
	bool isanimated = true;



};


#endif