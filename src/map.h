#ifndef MAP_H
#define MAP_H

#include "mesh.h"
#include "texture.h"
#include "shader.h"
#include "camera.h"



class Map
{





public:

	Map();
	~Map();


	Map(Camera * camera, float* time);
	Camera *camera, *camera2D;
	float* time;

};


#endif
