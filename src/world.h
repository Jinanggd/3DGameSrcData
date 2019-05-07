#ifndef WORLD_H
#define WORLD_H

#include "mesh.h"
#include "texture.h"
#include "entityMesh.h"
#include "entityLight.h"
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

	bool load();
	bool save();

	//static World* Instance();
	Camera * camera;
	float* time;
	Shader* current_shader;
	std::vector<EntityMesh> entities;
	std::vector<EntityLight> lights;
	Matrix44 m;

	gamestate mygameState;

	Mesh plane;
	EntityMesh Skybox;

	Shader* plane_shader = NULL;

//private:
//	static World* instance;
};


#endif
