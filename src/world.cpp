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
	initWorld();
	//initTree();
	//initAirplane();

	//Player = new EntityMesh(mat_types::airplane);
	Player = new EntityPlayer();

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

	Matrix44 m;
	for (int i = 0; i < entities.size(); ++i) {
		m.setTranslation(entities[i].model.getTranslation().x, entities[i].model.getTranslation().y, entities[i].model.getTranslation().z);
		Vector3 world_center = m*entities[i].mesh->box.center;
		//std::cout << entities[i].model.getTranslation().x << " " << entities[i].model.getTranslation().y <<" " << entities[i].model.getTranslation().z << std::endl;

		if (!(this->camera->testBoxInFrustum(world_center, entities[i].mesh->box.halfsize) == CLIP_OUTSIDE))
		{

			current_shader = entities[i].mat.shader;

			current_shader->enable();

			current_shader->setUniform("u_viewprojection", camera->viewprojection_matrix);

			current_shader->setUniform("u_time", *time);

			entities[i].render();

			current_shader->disable();


		}

	
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

void World::initProps() {
	
	std::vector<Vector3> positions;

	for (int i = 0; i < mask->image.width; i += 150) {
		float px = mapping(0, mask->image.width, -1024, 1024, i);

		for (int j = 0; j < mask->image.height; j += 150) {

			float pz = mapping(0, mask->image.width, -1024, 1024, j);
			float py = mask->image.getPixel(j, i).x / 255.0f * 40.0f;

			//Trees
			if (mask->image.getPixel(j, i).x > 100 && mask->image.getPixel(j, i).x < 200) {
				EntityMesh m = EntityMesh(mat_types::tree);
				m.model.setTranslation(px, py, pz);
				m.model.scale(5, 5, 5);
				entities.push_back(m);
			}
			//Building
			if (mask->image.getPixel(j, i).x > 200 && ( j < mask->image.height-100) && (i<mask->image.width-300)) {
				if (j % 3 == 0) {
					EntityMesh m = EntityMesh(mat_types::tower);
					m.model.setTranslation(px, py, pz);
					m.model.scale(1, 1, 1);
					entities.push_back(m);
				}

				if (j % 7 == 0) {

				}
				else {
					EntityMesh h = EntityMesh(mat_types::house);
					//Can add random rotation
					h.model.setTranslation(px, py, pz);
					h.model.scale(1, 1, 1);
					entities.push_back(h);
				}
			}

		}
	}
		

	// Creas entities arboles
	//for (int i = 0; i < positions.size(); i++) {

	//	EntityMesh m = EntityMesh(mat_types::tree);

	//	m.model.setTranslation(positions[i].x, positions[i].y, positions[i].z);
	//	m.model.scale(5, 5, 5);
	//	entities.push_back(m);
	//}

	


}

void World::printCamPos()
{
	std::cout <<"("<< camera->eye.x << "," << camera->eye.y <<"," << camera->eye.z << ")"<< std::endl;
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

void World::initWorld()
{
	//Terrain 
	//plane.createPlane(1024);
	plane.createSubdividedPlane(1024 * 2, 128, true);
	plane_shader = Shader::Get("data/shaders/heightmap.vs", "data/shaders/plane_texture.fs");
	water = new	EntityWater();
	cloud = new EntityCloud();
	mask = Texture::Get("data/heightmap.tga");
	mask->image.loadTGA("data/heightmap.tga");

	initProps();


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