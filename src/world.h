#ifndef WORLD_H
#define WORLD_H

#include "mesh.h"
#include "texture.h"
#include "entityMesh.h"
#include "entityLight.h"
#include "entityWater.h"
#include "entityCloud.h"
#include "entityPlayer.h"
#include "entityAI.h"
#include "shader.h"
#include "GUI.h"
#include "map.h"


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

	void mapinit();

	void rendermap();

	//Renders
	void renderentities();
	void render_props();
	void render_cannons();
	void render_buildables();
	void render_titan();
	void render_player();
	void renderplane();
	void renderSkybox();
	void renderGUI();
	void renderBlendings();

	//Inits
	void initProps();
	void initWorld();
	void CheckInit();
	void initGUIs();
	void initPlayer();
	void SpawnTitans();

	//Update
	void setAllGUItofalse();
	void update(float dt);


	void updateBullets(int index, Vector3 position);
	void removeBullet(int index);

	//Utils
	float mapping(float start1, float stop1, float start2, float stop2, float value);
	void printCamPos();
	void sortBlendingObjects(EntityMesh m);
	void isClear();


	bool load();
	bool save();

	//static World* Instance();
	Camera * camera;
	Camera * camera2D;
	float* time;
	bool instruction_help = false;
	Shader* current_shader;

	std::vector<EntityMesh> props;
	std::vector<EntityMesh> bullets_and_cannon;
	std::vector<EntityMesh> buildables;
	std::vector<EntityMesh> blendings;
	std::vector<GUI> GUIs;
	std::vector<EntityAI> Titans;

	//int shootedBullet;
	std::vector<int>shootedBullet;
	std::vector<EntityLight> lights;
	Matrix44 m;
	Texture* mask;

	gamestate mygameState;

	EntityWater* water;
	EntityMesh* explosion;
	EntityCloud* cloud;

	Map map;
	Mesh plane;
	EntityMesh Skybox;
	EntityPlayer* Player;
	std::vector<EntityPlayer> Players;
	EntityAI* Titan;

	Shader* plane_shader = NULL;
	Texture* plan_texture = NULL;

	std::vector<Vector3> spawnzones;
	int round = 0;
	int numbersofTitans[1] = { 5 };
	float initSpawnTime= -1;
	float cdSpawn = 5.0f;
	bool cleared = false;
	
	

//private:
//	static World* instance;
};


#endif
