#include "world.h"
#include <fstream>
#include <iostream>

//World* World::Instance() {
//	return instance;
//}
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

	//plane.createPlane(1024);
	plane.createSubdividedPlane(1024, 256, true);
	plane_shader = Shader::Get("data/shaders/heightmap.vs", "data/shaders/plane_texture.fs");

}


void World::renderSkybox() {

	Skybox = EntityMesh(Mesh::Get("data/sphere.obj"), mat_types::sky);

	current_shader = Skybox.mat.shader;

	current_shader->enable();


	Skybox.model.translate(camera->eye.x, camera->eye.y, camera->eye.z);
	Skybox.model.scale(100, 100, 100);

	current_shader->setUniform("u_color", Vector4(1, 1, 1, 1));
	current_shader->setUniform("u_viewprojection", camera->viewprojection_matrix);
	current_shader->setUniform("u_model", Skybox.model);
	current_shader->setUniform("u_time", *time);

	Skybox.render();

	current_shader->disable();

}

void World::renderentities()
{


	for (int i = 0; i < entities.size(); ++i) {

		current_shader = entities[i].mat.shader;

		current_shader->enable();

		current_shader->setUniform("u_viewprojection", camera->viewprojection_matrix);

		current_shader->setUniform("u_time", *time);

		entities[i].render();

		current_shader->disable();
	
	}


}

void World::renderplane() {

	current_shader = plane_shader;

	current_shader->enable();

	//m.translate(0, 0, 0);
	m.setIdentity();
	
	current_shader->setUniform("u_color", Vector4(1, 1, 1, 1));
	current_shader->setUniform("u_viewprojection", camera->viewprojection_matrix);
	current_shader->setUniform("u_texture_grass", Texture::Get("data/grass.tga"));
	current_shader->setUniform("u_texture_rock", Texture::Get("data/rocks.tga"));
	current_shader->setUniform("u_texture_mask", Texture::Get("data/heightmap.tga"));
	current_shader->setUniform("u_texture_water", Texture::Get("data/agua.tga"));
	current_shader->setUniform("u_model", m);
	current_shader->setUniform("u_time", *time);
	plane.render(GL_TRIANGLES);


	current_shader->disable();


}



bool World::load() {

	char* filename = "data/gamestate.bin";

	FILE * file = fopen(filename, "rb");

	if (file == NULL)
	{
		std::cerr << "::readFile: file not found " << filename << std::endl;
		return false;
	}
	

	fread(&mygameState, sizeof(mygameState), 1, file);

}

bool World::save() {

	char* filename = "data/gamestate.bin";

	FILE * file = fopen(filename, "wb");

	fwrite(&mygameState, sizeof(mygameState), 1, file);

	return 1;
}