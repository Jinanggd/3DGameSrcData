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

	void render(float time);
	void render(Camera* cam);
	void Init(Camera* cam);

	void update(float dt);
	void updateCamera();
	void updateMatrix();
	void animateCharacter();

	void setPosition(float x, float y, float z);

	Vector3 getLocalVector(Vector3 v);
	Vector3 current_position;
	float pitch;
	float yaw;
	float speed;

	enum direction {
		KEY_UP,
		KEY_DOWN,
		KEY_RIGHT,
		KEY_LEFT,
	}direction;

	Mesh* mesh;
	Animation* anim;
	Material mat;
	std::vector<Matrix44> bone_matrices;
	Camera * camera;
	bool isanimated = false;
	

};


#endif
