#include "map.h"
#include <fstream>
#include <iostream>


//World* World::Instance() {
//	return instance;
//}
Map::Map()
{
}


Map::~Map()
{
}

Map::Map(Camera * camera, EntityPlayer* player, float* time)
{
	this->camera = camera;

	this->camera2D = new Camera();
	this->camera2D->setOrthographic(0, 600, 0, 400, -1, 1);
	this->time = time;
	this->player = player;

	//Load all the GUIs

	this->shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
	this->texture = Texture::Get("data/rocks.tga");



}

void Map::update() {

	Vector3 eye = player->current_position;

	eye.y += 600;

	Vector3 center = player->current_position;

	this->camera->lookAt(eye,center,Vector3(1,0,0));
	this->camera2D->lookAt(eye, center, Vector3(1, 0, 0));

}




