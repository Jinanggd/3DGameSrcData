#ifndef MAP_H
#define MAP_H

#include "mesh.h"
#include "texture.h"
#include "shader.h"
#include "camera.h"
#include "entityPlayer.h"
#include "entityAI.h"


class Map
{





public:

	Map();
	~Map();


	Map(Camera * camera, EntityPlayer* player, float* time);
	void update();
	void render();
	void renderEntity(Shader *current_shader, Vector4 color, EntityPlayer *Player);
	void renderEntity(Shader *current_shader, Vector4 color, EntityAI *Player);
	void renderbuildablecanon(std::vector<EntityMesh> b, std::vector<EntityMesh> c);
	Camera *camera, *camera2D;
	float* time;
	float height;
	Shader *shader;
	Texture *texture;
	EntityPlayer *player;

};


#endif
