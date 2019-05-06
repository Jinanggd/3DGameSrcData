#include "world.h"



World::World()
{
}


World::~World()
{
}

World::World(Camera * camera, float* time)
{
	this->camera = camera;
	this->time = time;

}

void World::render()
{


	for (int i = 0; i < all_elements.size(); ++i) {

		current_shader = all_elements[i].mat.shader;

		current_shader->enable();

		current_shader->setUniform("u_viewprojection", camera->viewprojection_matrix);

		current_shader->setUniform("u_time", *time);

		all_elements[i].render();

		current_shader->disable();
	
	}


}


