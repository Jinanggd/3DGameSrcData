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
	plane.createSubdividedPlane(1024*2, 128, true);
	plane_shader = Shader::Get("data/shaders/heightmap.vs", "data/shaders/plane_texture.fs");
	water = new	EntityWater();
	cloud = new EntityCloud();
	mask = Texture::Get("data/heightmap.tga");
	mask->image.loadTGA("data/heightmap.tga");

	initTree();
	initAirplane();

	Player = new EntityMesh(mat_types::airplane);


}


void World::renderSkybox() {

	Skybox = EntityMesh( mat_types::sky);

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
	current_shader = Player->mat.shader;
	current_shader->enable();
	current_shader->setUniform("u_viewprojection", camera->viewprojection_matrix);

	current_shader->setUniform("u_time", *time);
	Player->render();

	current_shader->disable();
}

void World::renderplane() {

	//Rendering Terrain
	current_shader = plane_shader;

	current_shader->enable();

	//m.translate(0, 0, 0);

	m.setIdentity();
	m.translate(-1024, 0, -1024);
	
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

	//Rendering Water

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	current_shader = water->mat.shader;
	current_shader->enable();

	current_shader->setUniform("u_time", *time);
	current_shader->setUniform("u_viewprojection", camera->viewprojection_matrix);
	water->render();

	current_shader->disable();


	glDisable(GL_BLEND);
	
	/*

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_CONSTANT_ALPHA);
	glDepthMask(true);
	current_shader = cloud->mat.shader;
	current_shader->enable();

	current_shader->setUniform("u_time", *time);
	current_shader->setUniform("u_viewprojection", camera->viewprojection_matrix);
	cloud->render(camera);

	current_shader->disable();
	glDisable(GL_BLEND);
	glDepthMask(false);*/
}

void World::initTree() {
	
	std::vector<Vector3> positions;

	for(int i = 0; i< mask->image.width; i+=100)

		for (int j =0; j < mask->image.height; j+=100) {
			
			float px = mapping(0, mask->image.width, -1024, 1024, i);

			
		
			if (mask->image.getPixel(j, i).x >  100) {
				
				float pz = mapping(0, mask->image.width, -1024, 1024, j);
				float py = mask->image.getPixel(j, i).x/255.0f * 40.0f;
				positions.push_back(Vector3 (px, py, pz));

			}

		}

	// Creas entities arboles
	for (int i = 0; i < positions.size(); i++) {

		EntityMesh m = EntityMesh(mat_types::tree);

		m.model.setTranslation(positions[i].x, positions[i].y, positions[i].z);
		m.model.scale(5, 5, 5);
		entities.push_back(m);
	}

	


}

float World::mapping(float start1,float stop1, float start2,float stop2,float value) {
	
	float outgoing =
		start2 + (stop2 - start2) * ((value - start1) / (stop1 - start1));
	//std::cout << value << " " << outgoing <<std::endl;
	return outgoing;
}


void World::initAirplane() {

	EntityMesh plane = EntityMesh(mat_types::airplane);
	plane.model.translate(0, 100, 0);
	plane.model.scale(30, 30, 30);
	//plane.camera = new Camera();
	//Vector3 eye = plane.model *   Vector3(0, 100, 0);
	//Vector3 center = plane.model * Vector3(0, 0, 0);
	//camera->lookAt(eye, center, Vector3(0, 1, 0));
	//entities.push_back(plane);

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