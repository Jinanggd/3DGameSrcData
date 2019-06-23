#ifndef MAP_H
#define MAP_H

#include "mesh.h"
#include "texture.h"
#include "shader.h"
#include "camera.h"
#include "entityPlayer.h"


class Map
{





public:

	Map();
	~Map();


	Map(Camera * camera, EntityPlayer* player, float* time);
	void update();
	void render();
	Camera *camera, *camera2D;
	float* time;
	float height;
	Shader *shader;
	Texture *texture;
	EntityPlayer *player;

};


#endif
