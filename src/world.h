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


	void renderentities();
	void renderplane();
	void renderSkybox();
	void renderGUI();
	void renderBlendings();

	void initProps();
	void initAirplane();
	void initWorld();
	void initGUIs();
	void initPlayer();
	void SpawnTitans();

	void printCamPos();
	float mapping(float start1, float stop1, float start2, float stop2, float value);

	void update(float dt);

	void updateBullets(int index, Vector3 position);
	void removeBullet(int index);
	void sortBlendingObjects(EntityMesh m);

	bool dist( EntityMesh& lhs,  EntityMesh& rhs);

	void setAllGUItofalse();

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

	int shootedBullet;
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
	
	

//private:
//	static World* instance;
};


#endif
