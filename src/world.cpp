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
	Player = new EntityPlayer(time);

	for (int i = 0; i < 10; i++) {
	
		EntityPlayer p =  EntityPlayer(time);
		Players.push_back(p);
	
	}

	Titan = new EntityAI(time, &Player->current_position);
	initWorld();
	//initTree();
	//initAirplane();

	//Player = new EntityMesh(mat_types::airplane);
	

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
	for (int i = 0; i < props.size(); ++i) {
		m.setTranslation(props[i].model.getTranslation().x, props[i].model.getTranslation().y, props[i].model.getTranslation().z);
		Vector3 world_center = m*props[i].mesh->box.center;
		//std::cout << props[i].model.getTranslation().x << " " << props[i].model.getTranslation().y <<" " << props[i].model.getTranslation().z << std::endl;

		if (!(this->camera->testSphereInFrustum(world_center, 50) == CLIP_OUTSIDE))
		{

			current_shader = props[i].mat.shader;

			current_shader->enable();

			current_shader->setUniform("u_viewprojection", camera->viewprojection_matrix);

			current_shader->setUniform("u_time", *time);

			props[i].render();

			current_shader->disable();


		}

	
	}

	current_shader = Player->mat.shader;
	current_shader->enable();
	current_shader->setUniform("u_viewprojection", camera->viewprojection_matrix);
	current_shader->setUniform("u_time", *time);
	Player->render(*time);
	Titan->render();
	current_shader->disable();

	//current_shader = Player->actionplane.mat.shader;


	//current_shader->enable();
	//current_shader->setUniform("u_viewprojection", camera->viewprojection_matrix);
	//current_shader->setUniform("u_time", *time);
	//current_shader->setUniform("u_color", Vector4(1, 1, 1, 1));
	//current_shader->setUniform("u_texture", Player->actionplane.mat.texture);
	//current_shader->setUniform("u_model", Player->actionplane.model);
	//Player->actionplane.m.render(GL_TRIANGLES);

	//current_shader->disable();

	//current_shader = Player->hpbar.mat.shader;
	//current_shader->enable();
	//current_shader->setUniform("u_viewprojection", camera->viewprojection_matrix);
	//current_shader->setUniform("u_time", *time);
	//current_shader->setUniform("u_color", Vector4(1, 1, 1, 1));
	//current_shader->setUniform("u_texture", Player->hpbar.mat.texture);
	//current_shader->setUniform("u_model", Player->hpbar.model);
	//Player->hpbar.m.render(GL_TRIANGLES);
	//current_shader->disable();
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
	current_shader->setUniform("u_texture_grass", Texture::Get("data/dry_grass.tga"));
	current_shader->setUniform("u_texture_rock", Texture::Get("data/terrain.tga"));
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
	
	//Here we will spawn trees, buildings, set player position and the Titans positions
	bool setPlayerPos = true;
	for (int i = 0; i < mask->image.width; i += 200) {
		float px = mapping(0, mask->image.width, -1024, 1024, i);

		for (int j = 0; j < mask->image.height; j += 200) {

			float pz = mapping(0, mask->image.width, -1024, 1024, j);
			float py = mask->image.getPixel(j, i).x / 255.0f * 40.0f;
			int randtext = rand() % 4 + 1;
			float randscale = random() + 1.0f;
			if (randscale == 0) randscale = 1.5f;
			//Trees
			if (mask->image.getPixel(j, i).x > 100 && mask->image.getPixel(j, i).x < 162) {
				EntityMesh m = EntityMesh(mat_types::tree);
				m.model.setTranslation(px, py, pz);
				m.model.scale(7, 7, 7);
				props.push_back(m);
			}
			//Building and other materials
			if (mask->image.getPixel(j, i).x >= 162 && mask->image.getPixel(j,i).x !=255 && mask->image.getPixel(j,i).z !=255) {
				if (j % 3 == 0) {
					EntityMesh m = EntityMesh(mat_types::tower);
					m.model.setTranslation(px, py, pz);
					m.model.scale(randscale, randscale, randscale);
					std::string filename = "data/house_" + std::to_string(randtext) + ".tga";
					const char *c = filename.c_str();
					m.mat.texture = Texture::Get(c);
					props.push_back(m);
				}

				if (j % 7 == 0) {

				}
				else {
					EntityMesh h = EntityMesh(mat_types::house);
					//Can add random rotation
					h.model.setTranslation(px, py, pz);
					h.model.scale(randscale, randscale, randscale);
					std::string filename = "data/house_" + std::to_string(randtext) + ".tga";
					const char *c = filename.c_str();
					h.mat.texture = Texture::Get(c);
					props.push_back(h);
				}
			}

			if (mask->image.getPixel(j, i).x == 255 && setPlayerPos) {
				float characterpy = py * 0.63f;
				float bulletpy = py * 0.65f;
				EntityMesh b = EntityMesh(mat_types::bullet);
				
				b.model.setTranslation(px, bulletpy, pz+20);
				b.index_propsvector = props.size();
				props.push_back(b);
				bullets_and_cannon.push_back(b);
				

				b = EntityMesh(mat_types::cannon);
				b.model.setTranslation(px, bulletpy, pz + 50);
				b.model.scale(2, 2, 2);
				b.index_propsvector = props.size();
				props.push_back(b);
				bullets_and_cannon.push_back(b);
				
				this->Player->setPosition(px, characterpy, pz);

				Titan->model.setTranslation(px, characterpy , pz+20);
				
				for (int i = 0; i < Players.size(); i++) {


					Players.at(i).setPosition(random(20), characterpy, random(10));
				
				}

				setPlayerPos = false;

				
			}

			if (mask->image.getPixel(j, i).z == 255) {
				// Titan Spawn
				
			}

		}
	}
		

	// Creas props arboles
	//for (int i = 0; i < positions.size(); i++) {

	//	EntityMesh m = EntityMesh(mat_types::tree);

	//	m.model.setTranslation(positions[i].x, positions[i].y, positions[i].z);
	//	m.model.scale(5, 5, 5);
	//	props.push_back(m);
	//}

	


}

void World::printCamPos()
{
	//std::cout <<"("<< Player->current_position.x << "," << Player->current_position.y <<"," << Player->current_position.z << ")"<< std::endl;
	std::cout << "(" << camera->eye.x << "," << camera->eye.y << "," << camera->eye.z << ")" << std::endl;
	std::cout << "(" << camera->center.x << "," << camera->center.y << "," << camera->center.z << ")" << std::endl;
	std::cout << "(" << bullets_and_cannon[1].model.getTranslation().x << "," << bullets_and_cannon[1].model.getTranslation().y << "," << bullets_and_cannon[1].model.getTranslation().z << ")" << std::endl;
	//std::cout << "(" << Player->camera << "," << Player->current_position.y << "," << Player->current_position.z << ")" << std::endl;

	//std::cout << "Pitch: " << Player->pitch << std::endl;
}

float World::mapping(float start1,float stop1, float start2,float stop2,float value) {
	
	float outgoing =
		start2 + (stop2 - start2) * ((value - start1) / (stop1 - start1));
	//std::cout << value << " " << outgoing <<std::endl;
	return outgoing;
}

void World::updateBullets(int index, Vector3 position)
{
	bullets_and_cannon[index].model.setTranslation(position.x,position.y,position.z);
	props[bullets_and_cannon[index].index_propsvector].model.setTranslation(position.x, position.y, position.z);
}


void World::initAirplane() {

	EntityMesh plane = EntityMesh(mat_types::airplane);
	plane.model.translate(0, 100, 0);
	plane.model.scale(30, 30, 30);
	//plane.camera = new Camera();
	//Vector3 eye = plane.model *   Vector3(0, 100, 0);
	//Vector3 center = plane.model * Vector3(0, 0, 0);
	//camera->lookAt(eye, center, Vector3(0, 1, 0));
	//props.push_back(plane);

}

void World::initWorld()
{
	//Terrain 
	//plane.createPlane(1024);
	plane.createSubdividedPlane(1024 * 2, 128, true);
	plane_shader = Shader::Get("data/shaders/heightmap.vs", "data/shaders/plane_texture.fs");
	water = new	EntityWater();
	cloud = new EntityCloud();
	mask = Texture::Get("data/spawn_mask.tga");
	mask->image.loadTGA("data/spawn_mask.tga");

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