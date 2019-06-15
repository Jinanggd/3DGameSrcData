#ifndef ENTITYAI_H
#define ENTITYAI_H

#include "mesh.h"
#include "camera.h"
#include "entity.h"
#include "entityMesh.h"
#include "animation.h"



class EntityAI : public Entity
{




public:
	EntityAI();
	EntityAI(float * time);
	EntityAI(float * time, Vector3 * target);

	~EntityAI();


	void render(float time);
	void render();

	void Init(Camera* cam);

	void update(float dt, std::vector<EntityMesh> props);
	void updatedirection(float dt, std::vector<EntityMesh>props);
	void NPCMovement(float dt, std::vector<EntityMesh>props);


	void checkCollision(std::vector<EntityMesh> props, Vector3 newpos, float dt);
	void updateItem(Matrix44 r, Vector3 dir);
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
	Vector3 velocity, direction;
	Vector3 *target;

	float speed;
	float pitch, yaw;
	Matrix44 initialmatrixCannon;
	float * time;
	float hp;

	Skeleton skeleton;
	Mesh* mesh;


	Animation* anim;
	Material mat;


	std::vector<Matrix44> bone_matrices;
	Camera * camera;
	bool isanimated = true;



};


#endif