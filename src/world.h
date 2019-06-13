#ifndef WORLD_H
#define WORLD_H

#include "mesh.h"
#include "texture.h"
#include "entityMesh.h"
#include "entityLight.h"
#include "entityWater.h"
#include "entityCloud.h"
#include "entityPlayer.h"
#include "shader.h"


class World
{


	struct gamestate {

		int a = 0;
		int b = 0;

	};


public:

	World();
	~World();


	World(Camera * camera, float* time);


	void renderentities();
	void renderplane();
	void renderSkybox();

	void initProps();
	void initAirplane();
	void initWorld();

	void printCamPos();
	float mapping(float start1, float stop1, float start2, float stop2, float value);

	void updateBullets(int index, Vector3 position);
	void removeBullet(int index);
	void shotBullet(int index, float dt, Vector3 direction);

	bool load();
	bool save();

	//static World* Instance();
	Camera * camera;
	float* time;
	Shader* current_shader;
	std::vector<EntityMesh> props;
	std::vector<EntityMesh> bullets_and_cannon;
	std::vector<EntityLight> lights;
	Matrix44 m;
	Texture* mask;

	gamestate mygameState;

	EntityWater* water;
	EntityCloud* cloud;


	Mesh plane;
	EntityMesh Skybox;
	EntityPlayer* Player;
	std::vector<EntityPlayer> Players;
	EntityPlayer* Titan;

	Shader* plane_shader = NULL;

//private:
//	static World* instance;
};


#endif
