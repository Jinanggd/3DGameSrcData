#ifndef WORLD_H
#define WORLD_H

#include "mesh.h"
#include "texture.h"
#include "object.h"
#include "shader.h"


class World
{


public:

	World();
	~World();


	World(Camera * camera, float* time);

	void render();

	Camera * camera;
	float* time;
	Shader* current_shader;
	std::vector<Object> all_elements;

};


#endif
