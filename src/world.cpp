#include "world.h"
#include "game.h"
#include <fstream>
#include <iostream>
#include <algorithm>
#include <random>
#include "utils.h"



//World* World::Instance() {
//	return instance;
//}

std::random_device rseed;
std::mt19937 rng(rseed());


World::World()
{
}


World::~World()
{
}

World::World(Camera * camera, float* time)
{
	this->camera = camera;
	this->camera2D = new Camera();
	this->camera2D->setOrthographic(0, 800, 0, 600, -1, 1);
	this->time = time;

	//Load all the GUIs
	//initGUIs();

	
	//for (int i = 0; i < 10; i++) {

	//	EntityPlayer p = EntityPlayer(time);
	//	Players.push_back(p);

	//}

	
	

	//initWorld();

	//mapinit();
	//initTree();
	//initAirplane();

	//Player = new EntityMesh(mat_types::airplane);


}


void World::mapinit() {

	Camera* cam = new Camera();
	cam->lookAt(Vector3(0.f, 100.f, 100.f), Vector3(100.f, 0.f, 0.f), Vector3(0.f, 1.f, 0.f)); //position the camera and point to 0,0,0
	cam->setPerspective(70.f, 1, 0.1f, 10000.f); //set the projection, we want to be perspective
	cam->enable();

	map = Map(cam, Player, time);

}

void World::rendermap() {

	glLineWidth(1);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND); glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	Matrix44 m;
	Matrix44 camera_matrix = map.camera->viewprojection_matrix;

	current_shader = map.shader;

	current_shader->enable();

	m.setIdentity();
	m.translate(-1024, 0, -1024);

	//render ground
	current_shader->setUniform("u_color", Vector4(1, 1, 1, 1));
	current_shader->setUniform("u_viewprojection", camera_matrix);
	current_shader->setUniform("u_texture_grass", Texture::Get("data/dry_grass.tga"));
	current_shader->setUniform("u_texture_rock", Texture::Get("data/terrain.tga"));
	current_shader->setUniform("u_texture_mask", Texture::Get("data/heightmap.tga"));
	current_shader->setUniform("u_texture_water", Texture::Get("data/agua.tga"));
	current_shader->setUniform("u_model", m);
	current_shader->setUniform("u_time", *time);
	plane.render(GL_TRIANGLES);

	current_shader->disable();

	for (int i = 0; i < props.size(); ++i) {


		current_shader = Shader::Get("data/shaders/basic_map.vs", "data/shaders/basic_map.fs");

		current_shader->enable();

		current_shader->setUniform("u_viewprojection", camera_matrix);

		current_shader->setUniform("u_time", *time);

		current_shader->setUniform("u_color", Vector4(1, 1, 1, 1));

		current_shader->setUniform("u_texture", props[i].mat.texture);
		current_shader->setUniform("u_model", props[i].model);

		props[i].mesh->render(GL_TRIANGLES);

		current_shader->disable();
		

	}	



	current_shader = Shader::getDefaultShader("flat");

	map.renderEntity(current_shader, Vector4(1, 1, 1, 1), Player);

	for (int i = 0; i < Titans.size(); i++) {
		map.renderEntity(current_shader, Vector4(1, 0, 0, 1), &Titans[i]);

	}

	map.renderbuildablecanon(buildables, bullets_and_cannon);
	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
}

void World::renderSkybox() {

	Skybox = EntityMesh(mat_types::sky);

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

void World::renderGUI()
{
	Matrix44 mGUI;
	mGUI.setIdentity();
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_DEPTH_TEST);
	for (int i = 0; i < GUIs.size(); i++) {
		if (GUIs[i].enable) {
			//We render the GUI if it is enabled
			current_shader = GUIs[i].shader;
			current_shader->enable();
			if (!GUIs[i].isHUD)
				current_shader->setUniform("u_viewprojection", camera2D->viewprojection_matrix);
			else
				current_shader->setUniform("u_viewprojection", mGUI);
			current_shader->setUniform("u_time", *time);
			GUIs[i].render();
			current_shader->disable();

		}
	}

	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
}

void World::renderBlendings()
{
	if (blendings.size() == 0) return;
	for (int i = 0; i < blendings.size(); ++i) {
		m.setTranslation(blendings[i].model.getTranslation().x, blendings[i].model.getTranslation().y, blendings[i].model.getTranslation().z);
		Vector3 world_center = m * blendings[i].mesh->box.center;
		if (!(this->camera->testSphereInFrustum(world_center, 50) == CLIP_OUTSIDE))
		{

			current_shader = blendings[i].mat.shader;

			current_shader->enable();

			current_shader->setUniform("u_viewprojection", camera->viewprojection_matrix);

			current_shader->setUniform("u_time", *time);

			blendings[i].render();

			current_shader->disable();

		}
	}
}

void World::renderentities()
{
	render_props();
	render_cannons();
	render_buildables();
	render_titan();
	render_player();

	//EXPLOSION -- No need frustrum
	current_shader = explosion->mat.shader;
	current_shader->enable();
	current_shader->setUniform("u_viewprojection", camera->viewprojection_matrix);
	current_shader->setUniform("u_time", *time);
	explosion->render();
	current_shader->disable();
}

void World::render_props()
{
	Matrix44 m;

	//PROPS
	glEnable(GL_DEPTH_TEST);
	for (int i = 0; i < props.size(); ++i) {
		m.setTranslation(props[i].model.getTranslation().x, props[i].model.getTranslation().y, props[i].model.getTranslation().z);
		Vector3 world_center = m * props[i].mesh->box.center;
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
}

void World::render_cannons()
{
	Matrix44 m;
	//BULLETS AND CANNON
	for (int i = 0; i < bullets_and_cannon.size(); ++i) {
		m.setTranslation(bullets_and_cannon[i].model.getTranslation().x, bullets_and_cannon[i].model.getTranslation().y, bullets_and_cannon[i].model.getTranslation().z);
		Vector3 world_center = m * bullets_and_cannon[i].mesh->box.center;
		if (!(this->camera->testSphereInFrustum(world_center, 50) == CLIP_OUTSIDE))
		{
			if (bullets_and_cannon[i].type == (int)mat_types::cannon) {
				current_shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
				current_shader->enable();
				current_shader->setUniform("u_viewprojection", camera->viewprojection_matrix);

				current_shader->setUniform("u_time", *time);

				bullets_and_cannon[i].munition.size() > 0 ?
					current_shader->setUniform("u_color", Vector4(0, 0.8, 0, 1)) :
					current_shader->setUniform("u_color", Vector4(1, 1, 1, 1));
				current_shader->setUniform("u_texture", bullets_and_cannon[i].mat.texture2);

				m.setTranslation(bullets_and_cannon[i].model.getTranslation() + Vector3(0, 2, 0));
				m.rotate(90 * DEG2RAD, Vector3(1, 0, 0));
				m.rotate(30 * *time*DEG2RAD, Vector3(0, 0, 1));
				m.scale(3, 3, 3);
				//m.rotate(sin(*time), Vector3(0, 1, 0));
				current_shader->setUniform("u_model", m);
				bullets_and_cannon[i].mesh2->render(GL_TRIANGLES);

				current_shader->disable();
			}

			current_shader = bullets_and_cannon[i].mat.shader;

			current_shader->enable();

			current_shader->setUniform("u_viewprojection", camera->viewprojection_matrix);

			current_shader->setUniform("u_time", *time);

			bullets_and_cannon[i].render();

			current_shader->disable();

		}
	}
}

void World::render_buildables()
{
	Matrix44 m;
	//BUILDABLES
	for (int i = 0; i < buildables.size(); ++i) {
		m.setTranslation(buildables[i].model.getTranslation().x, buildables[i].model.getTranslation().y, buildables[i].model.getTranslation().z);
		Vector3 world_center = m * buildables[i].mesh->box.center;
		if (!(this->camera->testSphereInFrustum(world_center, 50) == CLIP_OUTSIDE))
		{
			current_shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
			current_shader->enable();
			current_shader->setUniform("u_viewprojection", camera->viewprojection_matrix);

			current_shader->setUniform("u_time", *time);

			if (buildables[i].type == (int)mat_types::buildable)
				current_shader->setUniform("u_color", Vector4(1, 1, 1, 1));
			else if (buildables[i].type == (int)mat_types::tower1)
				current_shader->setUniform("u_color", Vector4(0, 0, 0.8f, 1.0));
			else if (buildables[i].type == (int)mat_types::tower2)
				current_shader->setUniform("u_color", Vector4(0.8f, 0, 0, 1.0));
			current_shader->setUniform("u_texture", buildables[i].mat.texture2);

			Vector3 t = buildables[i].model.getTranslation();
			m.setTranslation(t + Vector3(0, 2, 0));
			m.rotate(90 * DEG2RAD, Vector3(1, 0, 0));
			m.scale(5, 5, 5);
			m.rotate(30 * *time*DEG2RAD, Vector3(0, 0, 1));
			current_shader->setUniform("u_model", m);
			buildables[i].mesh2->render(GL_TRIANGLES);

			current_shader->disable();

			if (buildables[i].type == (int)mat_types::buildable)continue;
			current_shader = buildables[i].mat.shader;

			current_shader->enable();

			current_shader->setUniform("u_viewprojection", camera->viewprojection_matrix);

			current_shader->setUniform("u_time", *time);

			buildables[i].render();

			current_shader->disable();
			if (buildables[i].type == (int)mat_types::tower2)
				if (buildables[i].g.enable) {
					current_shader = buildables[i].g.shader;
					current_shader->enable();
					m.setIdentity();
					current_shader->setUniform("u_viewprojection", m);
					current_shader->setUniform("u_time", *time);
					buildables[i].g.render();
					current_shader->disable();
				}

		}
	}

}

void World::render_titan()
{
	Matrix44 m;
	//TITANS
	for (int i = 0; i < Titans.size(); i++) {
		m.setTranslation(Titans[i].model.getTranslation());
		Vector3 wc = m * Titans[i].mesh->box.center;
		if (!(this->camera->testSphereInFrustum(wc, 50) == CLIP_OUTSIDE)) {
			current_shader = Titans[i].mat.shader;
			current_shader->enable();
			current_shader->setUniform("u_viewprojection", camera->viewprojection_matrix);
			current_shader->setUniform("u_time", *time);
			Titans[i].render();
			current_shader->disable();
			if (Titans[i].hpbar.enable) {
				current_shader = Titans[i].hpbar.shader;
				current_shader->enable();
				m.setIdentity();
				current_shader->setUniform("u_viewprojection", m);
				current_shader->setUniform("u_time", *time);
				Titans[i].hpbar.render();
				current_shader->disable();
			}
			//current_shader = Titans[i].bbox.mat.shader;
			//current_shader->enable();
			//current_shader->setUniform("u_viewprojection", camera->viewprojection_matrix);
			//current_shader->setUniform("u_time", *time);
			//current_shader->setUniform("u_color", Vector4(1, 1, 1, 0.5));
			//Matrix44 s;
			//s.setTranslation(Titans[i].current_position);
			//s.setScale(8, 7, 5);
			//current_shader->setUniform("u_model", s*Titans[i].model);
			//current_shader->setUniform("u_texture", Titans[i].bbox.mat.texture);
			//Titans[i].bbox.mesh->render(GL_TRIANGLES);
			//current_shader->disable();
		}
	}
}

void World::render_player()
{
	Matrix44 m;
	//PLAYER
	current_shader = Player->mat.shader;
	current_shader->enable();
	current_shader->setUniform("u_viewprojection", camera->viewprojection_matrix);
	current_shader->setUniform("u_time", *time);
	Player->render(*time);
	current_shader->disable();
	if (Player->scope.enable) {
		glDisable(GL_DEPTH_TEST);
		current_shader = Player->scope.shader;
		current_shader->enable();
		m.setIdentity();
		current_shader->setUniform("u_viewprojection", m);
		current_shader->setUniform("u_time", *time);
		Player->scope.render();
		current_shader->disable();
		glEnable(GL_DEPTH_TEST);
		//glDisable(GL_BLEND);

		//drawText(5, 550, "Bullets available: " + std::to_string(Player->Cannon.munition.size()), Vector3(1, 1, 1), 4);

		
	}
	else if (Player->staminaBar.enable && Player->stamina > 0) {
		glDisable(GL_DEPTH_TEST);
		current_shader = Player->staminaBar.shader;
		current_shader->enable();
		current_shader->setUniform("u_viewprojection", camera2D->viewprojection_matrix);
		current_shader->setUniform("u_time", *time);
		current_shader->setUniform("u_color", Vector4(231.0f / 255.0f, 197.0f / 255.0f, 78.0f / 255.0f, 1.0));
		bool f = false;
		current_shader->setUniform("is3D", f);
		Player->staminaBar.size = Vector2(Player->stamina * 3, 20);
		Player->staminaBar.buildQuadJustified();
		Player->staminaBar.mesh->render(GL_TRIANGLES);
		current_shader->disable();
		current_shader->disable();

		glEnable(GL_DEPTH_TEST);
		//glDisable(GL_BLEND);
		//drawText(20, 530, "STAMINA", Vector3(1, 1, 1), 2);

		
	}
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
	current_shader->setUniform("u_texture_mask", plan_texture);
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

//Height positions - ARE HARDCODED LOL
void World::initProps() {

	//Here we will spawn trees, buildings, set player position and the Titans positions
	bool setPlayerPos = true;
	for (int i = 0; i < mask->image.width; i += 150) {
		float px = mapping(0, mask->image.width, -1024, 1024, i);

		for (int j = 0; j < mask->image.height; j += 200) {

			float pz = mapping(0, mask->image.width, -1024, 1024, j);
			float py = plan_texture->image.getPixel(j, i).x / 255.0f * 20.0f;
			float characterpy = py;
			float bulletpy = py+1.0f;
			float cannonpy = py;

			int randtext = rand() % 4 + 1;
			float randscale = random() + 1.0f;
			if (randscale == 0) randscale = 1.5f;

			if (mask->image.getPixel(j, i).z == 255 && mask->image.getPixel(j, i).x != 255) {
				spawnzones.push_back(Vector3(px, characterpy, pz));
			}
			//Building and other materials
			std::uniform_int<int>dist1(1, 15);
			if (mask->image.getPixel(j, i).y == 255 && mask->image.getPixel(j, i).x != 255) {
				EntityMesh b;
				float rotation_angle;
				const char *c;
				std::string filename;
				switch (dist1(rng))
				{
				case 1: case 2:
					//2 cannons
					//1 cannons , 1 buildable, 5 bullets, 2 houses
					for (int k = 0; k < 2; k++) {
						rotation_angle = rand() % 360;
						b = EntityMesh(mat_types::cannon);
						b.model.setTranslation(px+20*(k+1), cannonpy, pz+20*(k+1));
						b.model.rotate(rotation_angle*DEG2RAD, Vector3(0, 1, 0));
						b.rotation = rotation_angle;
						b.model.scale(6, 6, 6);
						bullets_and_cannon.push_back(b);
					}
					
					for (int k = 0; k < 5; k++) {
						std::uniform_int<int>dist(-10, 10);
						b = EntityMesh(mat_types::bullet);
						b.model.setTranslation(px +dist(rng), bulletpy, pz + dist(rng));
						bullets_and_cannon.push_back(b);
					}
					
					break;
				case 3:
					rotation_angle = rand() % 360;
					b = EntityMesh(mat_types::cannon);
					b.model.setTranslation(px, cannonpy, pz);
					b.model.rotate(rotation_angle*DEG2RAD, Vector3(0, 1, 0));
					b.rotation = rotation_angle;
					b.model.scale(6, 6, 6);
					bullets_and_cannon.push_back(b);
					break;
				case 4: case 5:
					for (int k = 0; k < 1; k++) {
						b = EntityMesh(mat_types::buildable);
						b.model.setTranslation(px + 20 * (k+1), py, pz + 20 * (k+1));
						b.model.scale(4, 7, 4);
						buildables.push_back(b);
					}
					rotation_angle = rand() % 360;
					b = EntityMesh(mat_types::cannon);
					b.model.setTranslation(px, cannonpy, pz);
					b.model.rotate(rotation_angle*DEG2RAD, Vector3(0, 1, 0));
					b.rotation = rotation_angle;
					b.model.scale(6, 6, 6);
					bullets_and_cannon.push_back(b);


					break;
				case 6:
					for (int k = 0; k < 1; k++) {
						b = EntityMesh(mat_types::buildable);
						b.model.setTranslation(px + 20 * (k + 1), py, pz + 20 * (k + 1));
						b.model.scale(4, 7, 4);
						buildables.push_back(b);
					}
					for (int k = 0; k < 5; k++) {
						std::uniform_int<int>dist(-10, 10);
						b = EntityMesh(mat_types::bullet);
						b.model.setTranslation(px + dist(rng), bulletpy, pz + dist(rng));
						bullets_and_cannon.push_back(b);
					}
					break;
				case 7:
					b= EntityMesh(mat_types::house);
					//Can add random rotation
					b.model.setTranslation(px, py, pz);
					b.model.scale(randscale, randscale, randscale);
					filename = "data/house_" + std::to_string(randtext) + ".tga";
					c = filename.c_str();
					b.mat.texture = Texture::Get(c);
					props.push_back(b);

					b = EntityMesh(mat_types::tower);
					b.model.setTranslation(px, py, pz);
					b.model.scale(randscale, randscale, randscale);
					filename = "data/house_" + std::to_string(randtext) + ".tga";
					c = filename.c_str();
					b.mat.texture = Texture::Get(c);
					props.push_back(b);

					break;

				case 8:
					b = EntityMesh(mat_types::house);
					b.model.setTranslation(px, py, pz);
					b.model.scale(randscale, randscale, randscale);
					filename = "data/house_" + std::to_string(randtext) + ".tga";
					c = filename.c_str();
					b.mat.texture = Texture::Get(c);
					props.push_back(b);

					for (int k = 0; k < 5; k++) {
						std::uniform_int<int>dist(10, 50);
						b = EntityMesh(mat_types::bullet);
						b.model.setTranslation(px + dist(rng), bulletpy, pz + dist(rng));
						bullets_and_cannon.push_back(b);
					}
					break;

				case 9:
					for (int k = 0; k < 3; k++) {
						b = EntityMesh(mat_types::tree);
						b.model.setTranslation(px + 20 * (k + 1), py, pz + 20 * (k + 1));
						b.model.scale(10, rand() % 15 + 8, 10);
						props.push_back(b);
					}

					rotation_angle = rand() % 360;
					b = EntityMesh(mat_types::cannon);
					b.model.setTranslation(px, cannonpy, pz);
					b.model.rotate(rotation_angle*DEG2RAD, Vector3(0, 1, 0));
					b.rotation = rotation_angle;
					b.model.scale(6, 6, 6);
					bullets_and_cannon.push_back(b);

					break;
				case 10: case 11:
					b = EntityMesh(mat_types::house);
					//Can add random rotation
					b.model.setTranslation(px, py, pz);
					b.model.scale(randscale, randscale, randscale);
					filename = "data/house_" + std::to_string(randtext) + ".tga";
					c = filename.c_str();
					b.mat.texture = Texture::Get(c);
					props.push_back(b);

					b = EntityMesh(mat_types::tower);
					b.model.setTranslation(px, py, pz);
					b.model.scale(randscale, randscale, randscale);
					filename = "data/house_" + std::to_string(randtext) + ".tga";
					c = filename.c_str();
					b.mat.texture = Texture::Get(c);
					props.push_back(b);

					break;
				case 12: case 13: 
					b = EntityMesh(mat_types::house);
					//Can add random rotation
					b.model.setTranslation(px, py, pz);
					b.model.scale(randscale, randscale, randscale);
					filename = "data/house_" + std::to_string(randtext) + ".tga";
					c = filename.c_str();
					b.mat.texture = Texture::Get(c);
					props.push_back(b);

					b = EntityMesh(mat_types::tower);
					b.model.setTranslation(px, py, pz);
					b.model.scale(randscale, randscale, randscale);
					filename = "data/house_" + std::to_string(randtext) + ".tga";
					c = filename.c_str();
					b.mat.texture = Texture::Get(c);
					props.push_back(b);
				case 14: 
					for (int k = 0; k < 4; k++) {
						rotation_angle = rand() % 360;
						b = EntityMesh(mat_types::cannon);
						b.model.setTranslation(px + 50 * (k + 1), cannonpy, pz + 20 * (k + 1));
						b.model.rotate(rotation_angle*DEG2RAD, Vector3(0, 1, 0));
						b.rotation = rotation_angle;
						b.model.scale(6, 6, 6);
						bullets_and_cannon.push_back(b);
					}

					for (int k = 0; k < 5; k++) {
						std::uniform_int<int>dist(-10, 10);
						b = EntityMesh(mat_types::bullet);
						b.model.setTranslation(px + dist(rng), bulletpy, pz + dist(rng));
						bullets_and_cannon.push_back(b);
					}

					break;
				default:
					for (int k = 0; k < 5; k++) {
						std::uniform_int<int>dist(-10, 10);
						b = EntityMesh(mat_types::bullet);
						b.model.setTranslation(px + dist(rng), bulletpy, pz + dist(rng));
						bullets_and_cannon.push_back(b);
					}
					//Sapwn nothing
					break;
				}
			}

			if (mask->image.getPixel(j, i).x == 255 &&  mask->image.getPixel(j,i).y != 255 && setPlayerPos) {
				
				std::uniform_int<int>dist(-20, 20);
				for (int k = 0; k < 5; k++) {
					EntityMesh b = EntityMesh(mat_types::bullet);

					b.model.setTranslation(px+dist(rng) + k, bulletpy, pz+ dist(rng) +k);
					b.index_propsvector = props.size();
					//props.push_back(b);
					bullets_and_cannon.push_back(b);
				}
				
				this->Player->setPosition(px, characterpy, pz);
				
				//EntityAI t = EntityAI(time, Player->model);
				//t.setPosition(px, Player->current_position.y + 10, pz + 20);
				//Titans.push_back(t);

				//EntityMesh a = EntityMesh(mat_types::cannon);
				//	a.model.setTranslation(px, cannonpy, pz -30);
				//a.model.scale(6, 6, 6);
				//a.rotation = 0;
				//bullets_and_cannon.push_back(a);




				setPlayerPos = false;


			}

		}
	}
	CheckInit();
}

void World::initGUIs() {
	GUI g = GUI(Vector2(800 / 2, 600 / 2), Vector2(800 / 1.5f, 600 / 1.5f), true, GUI_Types::instruct_building);
	GUIs.push_back(g);
	g = GUI(Vector2(800 / 2, 600 / 2), Vector2(800 / 1.5f, 600 / 1.5f), true, GUI_Types::instruct_attack);
	GUIs.push_back(g);
	g = GUI(Vector2(800 / 2, 600 / 2), Vector2(800 / 1.5f, 600 / 1.5f), true, GUI_Types::instruct_titan);
	GUIs.push_back(g);
	g = GUI(Vector2(800 / 2, 600 / 2), Vector2(800 / 1.5f, 600 / 1.5f), true, GUI_Types::instruct_mov);
	GUIs.push_back(g);


	g = GUI(Vector2(800 / 2, 600 / 2), Vector2(800 / 2, 600 / 2), false, GUI_Types::BulletKeysNC);
	GUIs.push_back(g);
	g = GUI(Vector2(800 / 2, 600 / 2), Vector2(800 / 2, 600 / 2), false, GUI_Types::BulletKeysC);
	GUIs.push_back(g);
	g = GUI(Vector2(800 / 2, 600 / 2), Vector2(800 / 2, 600 / 2), false, GUI_Types::CannonKeysNC);
	GUIs.push_back(g);
	g = GUI(Vector2(200, 150), Vector2(200, 100), false, GUI_Types::CannonKeysC1);
	GUIs.push_back(g);
	g = GUI(Vector2(600, 150), Vector2(110, 90 ), false, GUI_Types::CannonKeysC2);
	GUIs.push_back(g);
	g = GUI(Vector2(800 / 2, 600 / 2), Vector2(800 / 2, 600 / 2), false, GUI_Types::Building);
	GUIs.push_back(g);
	g = GUI(Vector2(800 / 2, 600 / 2), Vector2(800 / 2, 600 / 2), false, GUI_Types::BuildingT1);
	GUIs.push_back(g);
	g = GUI(Vector2(800 / 2, 600 / 2), Vector2(800 / 1.5f, 600 / 1.5f), false, GUI_Types::instruct_help);
	GUIs.push_back(g);
	g = GUI(Vector2(800 -70, 50), Vector2(50, 50), true, GUI_Types::OverallKeys);
	GUIs.push_back(g);
	
	
}

void World::initPlayer()
{
	Player = new EntityPlayer(time);
}

void World::SpawnTitans()
{
	//Pending CHECK
	if (Titans.size() > numbersofTitans[round] || initSpawnTime <0)
		return;

	float currenttime = *time - initSpawnTime;

	if (currenttime > cdSpawn) {
		initSpawnTime = *time;
		int index = rand() % spawnzones.size();
		EntityAI t = EntityAI(time, Player->model);
		t.setPosition(spawnzones[index].x, Player->current_position.y+10, spawnzones[index].z);
		Titans.push_back(t);
	}
	//EntityAI t = EntityAI(time, Player->model);
	////Titan = new EntityAI(time, Player->model);
	//t.setPosition(Player->current_position.x - 30, Player->current_position.y, Player->current_position.z + 70);
	//Titans.push_back(t);
}

void World::printCamPos()
{
	std::cout << "(" << camera->eye.x << "," << camera->eye.y << "," << camera->eye.z << ")" << std::endl;
	std::cout << "(" << camera->center.x << "," << camera->center.y << "," << camera->center.z << ")" << std::endl;
}

float World::mapping(float start1, float stop1, float start2, float stop2, float value) {

	float outgoing =
		start2 + (stop2 - start2) * ((value - start1) / (stop1 - start1));
	return outgoing;
}

void World::update(float dt)
{
	SpawnTitans();
	//Bullet shooted

	if (shootedBullet.size() > 0) {
		for (int i = 0; i < shootedBullet.size(); i++) {
			Vector3 currentposition = bullets_and_cannon[shootedBullet[i]].model.getTranslation();


			if (abs(currentposition.x) > 3000 || abs(currentposition.y) > 2000 || abs(currentposition.z) > 3000) {
				// Explosion GUI
				removeBullet(shootedBullet[i]);
				std::cout << "Too far away" << std::endl;
				return;
			}
			Vector3 movement = 100.0f*dt*bullets_and_cannon[shootedBullet[i]].Direction;

			movement.y += -0.5f* (*time - bullets_and_cannon[shootedBullet[i]].initial_time)*(*time - bullets_and_cannon[shootedBullet[i]].initial_time);
			
			
			Vector3 newposition = currentposition + movement;
			Vector3 dir = (newposition - currentposition);
			dir.normalize();
			float distance = (newposition - currentposition).length();

			Vector3 collisionpoint, collisionnormal;

			//Matrix44 s;
			//s.setTranslation(Titans[i].current_position);
			//s.setScale(8, 7, 5);
			//Search on Titans vector
			for (int j = 0; j < Titans.size(); j++) {
				Matrix44 SS;
				SS.setScale(8, 7, 5);
				if (Titans[j].bbox.mesh->testRayCollision(SS*Titans[j].model, currentposition, dir,
					collisionpoint, collisionnormal, distance)) {
					Vector3 d = bullets_and_cannon[shootedBullet[i]].Direction;
					d.y = 0;
					Vector3 point_explosion = collisionpoint - d * 5;
					explosion->model.setTranslation(point_explosion.x, point_explosion.y, point_explosion.z);
					explosion->explosion_initial_time = *time;
					Titans[j].substractLife();
					Titans[j].animtime = *time;
					removeBullet(shootedBullet[i]);
					isClear();
					return;


				}
			}
			//Search on propsvector
			//Hacer con test ray collision
			for (int j = 0; j < props.size(); j++) {

				//A Ray from the current point to the new position
				if (props[j].mesh->testRayCollision(props[j].model, currentposition, dir,
					collisionpoint, collisionnormal, distance)) {
					//Explosion GUI
					Vector3 d = bullets_and_cannon[shootedBullet[i]].Direction;
					d.y = 0;
					Vector3 point_explosion = collisionpoint - d * 5;
					explosion->model.setTranslation(point_explosion.x, point_explosion.y, point_explosion.z);
					explosion->explosion_initial_time = *time;
					//Remove the bullet from the the vectors
					removeBullet(shootedBullet[i]);
					std::cout << "Collision contra props" << std::endl;
					return;
				}

			}


			for (int j = 0; j < buildables.size(); j++) {

				//A Ray from the current point to the new position
				if (buildables[j].mesh->testRayCollision(buildables[j].model, currentposition, dir,
					collisionpoint, collisionnormal, distance)) {
					//Explosion GUI
					Vector3 d = bullets_and_cannon[shootedBullet[i]].Direction;
					d.y = 0;
					Vector3 point_explosion = collisionpoint - d * 5;
					explosion->model.setTranslation(point_explosion.x, point_explosion.y, point_explosion.z);
					explosion->explosion_initial_time = *time;
					//Remove the bullet from the the vectors
					removeBullet(shootedBullet[i]);
					std::cout << "Collision contra buildables" << std::endl;
					return;
				}

			}


			bullets_and_cannon[shootedBullet[i]].model.setTranslation(newposition.x, newposition.y, newposition.z);
			//props[bullets_and_cannon[shootedBullet[i]].index_propsvector].model.setTranslation(newposition.x, newposition.y, newposition.z);
			//props[bullets_and_cannon[index].index_propsvector].model.scale(50, 50, 50);
		}
		
	}

	//update GUIs
	Matrix44 R;
	R.setRotation(Player->yaw*DEG2RAD, Vector3(0, 1, 0));
	for (int i = 0; i < GUIs.size(); i++) {
		if (!GUIs[i].enable || GUIs[i].type < (int)GUI_Types::BulletKeysNC)continue;


		if (GUIs[i].type > (int)GUI_Types::instruct_titan && GUIs[i].type < (int)GUI_Types::Building) {	
			
			GUIs[i].setPositionfrom3D(Player->current_position + R*Vector3(-3, 13, 0), Vector2(0.1f,0.15f),
				this->camera->viewprojection_matrix);
		}
		else if (GUIs[i].type == (int)GUI_Types::Building) {
			GUIs[i].setPositionfrom3D(Player->current_position + R * Vector3(-5, 13, 0), Vector2(0.2f, 0.15f),
				this->camera->viewprojection_matrix);
		}
	}

	setAllGUItofalse();
	
	//Update GUI from buildables
	for (int i = 0; i < buildables.size(); i++) {
		if (buildables[i].type == (int)mat_types::tower2) {
			buildables[i].g.setPositionfrom3D(buildables[i].model.getTranslation() + Vector3(0, 60, 0),
				Vector2(0.3f*(1 / 3.0f*buildables[i].life), 0.03f), this->camera->viewprojection_matrix);
		}
	}

	//Update blendings vector
	blendings.clear();
	for (int i = 0; i < buildables.size(); i++) {
		if (buildables[i].type == (int)mat_types::buildable) {
			if(!buildables[i].upgrade(buildables[i].tobeupgrate, *time))
				sortBlendingObjects(buildables[i]);

		}
	}

	this->map.update();

	//Update explosion 
	if (explosion->explosion_initial_time > 0) {
		float difference = *time - explosion->explosion_initial_time;
		if (difference > explosion->duration) {
			explosion->explosion_initial_time = -1;
			explosion->model.setTranslation(0, -1000, 0);
		}
	}
	

}

void World::updateBullets(int index, Vector3 position)
{
	bullets_and_cannon[index].model.setTranslation(position.x,position.y,position.z);
}

void World::removeBullet(int index)
{
	int index_ = -1;
	for (int i = 0; i < shootedBullet.size(); i++) {
		if (shootedBullet[i] == index) {
			index_ = i;
			break;
		}
	}
	index_ >= 0 ? shootedBullet.erase(shootedBullet.begin() + index_): shootedBullet.erase(shootedBullet.begin());
	bullets_and_cannon.erase(bullets_and_cannon.begin() + index);

	for (int i = 0; i < bullets_and_cannon.size(); i++) {
		if (bullets_and_cannon[i].type == (int)mat_types::cannon) {
			if(bullets_and_cannon[i].munition.size() >0)

				for (int j = 0; j < bullets_and_cannon[i].munition.size(); j++) {
					if (bullets_and_cannon[i].munition[j] > index)

						bullets_and_cannon[i].munition[j]--;
				}
		}
	}
	
	if (Player->Cannon.munition.size() > 0) {
		for (int i = 0; i < Player->Cannon.munition.size(); i++) {
			if (Player->Cannon.munition[i] > index)
				Player->Cannon.munition[i]--;
		}
	}

	if(Player->CannonID > index)
		Player->CannonID--;

	for (int i = 0; i < shootedBullet.size(); i++) {
		if (shootedBullet[i] > index)
			shootedBullet[i]--;
	}
	//shootedBullet = -1;
}

void World::sortBlendingObjects(EntityMesh m)
{
	if (blendings.size() == 0) {
		blendings.push_back(m);
		return;
	}
	float distance = (m.model.getTranslation() - this->camera->eye).length();
	EntityMesh aux  = m;
	for (int i = 0; i < blendings.size(); i++) {
		float distance2 = (blendings[i].model.getTranslation() - this->camera->eye).length();
		if (distance > distance2) {
			EntityMesh aux2 = blendings[i];
			blendings[i] = aux;
			aux = aux2;
			distance = distance2;
		}
	}
	blendings.push_back(aux);
	
}

void World::isClear()
{
	for (int i = 0; i < Titans.size(); i++) {
		if (Titans[i].state != EntityAI::DEAD) {
			cleared = false;
			return;
		}
			
	}
	Game::instance->isOver = true;
	cleared = true;
}

void World::setAllGUItofalse()
{
	Vector3 position = Player->current_position;
	Vector3 collisionpoint, collisionnormal;
	for (int i = 4; i < 11; i++) {
		EntityMesh m;
		if (GUIs[i].index < 0)continue;
		switch (GUIs[i].type)
		{
		case (int)GUI_Types::BulletKeysC:
			m = bullets_and_cannon[GUIs[i].index];
			break;
		case (int)GUI_Types::BulletKeysNC:
			m = bullets_and_cannon[GUIs[i].index];
			break;
		case(int)GUI_Types::CannonKeysNC:
			m = bullets_and_cannon[GUIs[i].index];
			break;
		case(int)GUI_Types::Building:
			m = buildables[GUIs[i].index];
			break;
		case(int)GUI_Types::BuildingT1:
			m = buildables[GUIs[i].index];
			break;
		default:
			continue;
			break;
		}

		if (i == 5) {
			if(!m.mesh->testSphereCollision(m.model, position + Vector3(0, 2, 0), 9, collisionpoint, collisionnormal)) {
				GUIs[i].enable = false;
				GUIs[i].index = -1;
			}
		}
		else if(!m.mesh->testSphereCollision(m.model, position + Vector3(0, 2, 0), 3, collisionpoint, collisionnormal)) {
			GUIs[i].enable = false;
			GUIs[i].index = -1;
			if ((i == 6 && Player->iscarrying) || (i == 9 && Player->iscarrying)) {
				GUIs[5].enable = true;
			}
		}
		
	}
	
}

void World::initWorld()
{
	//Terrain 
	//plane.createPlane(1024);
	explosion = new EntityMesh(mat_types::explosion);
	explosion->model.setTranslation(0, -1000, 0);

	plan_texture = Texture::Get("data/characters/characters/titan.png");
	plan_texture = Texture::Get("data/Tower1.tga");
	plan_texture = Texture::Get("data/Tower2.tga");
	plane.createSubdividedPlane(1024 * 2, 128, true);
	plane_shader = Shader::Get("data/shaders/heightmap.vs", "data/shaders/plane_texture.fs");
	plan_texture = Texture::Get("data/heightmap.tga");
	plan_texture->image.loadTGA("data/heightmap.tga");
	water = new	EntityWater();
	mask = Texture::Get("data/spawn_mask.tga");
	mask->image.loadTGA("data/spawn_mask.tga");

	initProps();

}

void World::CheckInit()
{
	for (int i = 0; i < bullets_and_cannon.size(); i++) {
		if (bullets_and_cannon[i].type == (int)mat_types::cannon) {
			Vector3 cannonpos = bullets_and_cannon[i].model.getTranslation()+Vector3(0,2,0);
			for (int j = 0; j < props.size(); j++) {
				Vector3 collisionpoint, collisionnormal;
				if (props[j].mesh->testSphereCollision(props[j].model, cannonpos, 7, collisionpoint, collisionnormal) == true) {
					Vector3 push_away = normalize(collisionpoint - cannonpos);
					push_away.y = 0;
					bullets_and_cannon[i].model.translate(- 5*push_away);
				}
			}
		}
	}

	for (int i = 0; i < buildables.size(); i++) {
		if (buildables[i].type == (int)mat_types::cannon) {
			Vector3 cannonpos = buildables[i].model.getTranslation() + Vector3(0, 2, 0);
			for (int j = 0; j < props.size(); j++) {
				Vector3 collisionpoint, collisionnormal;
				if (props[j].mesh->testSphereCollision(props[j].model, cannonpos, 7, collisionpoint, collisionnormal) == true) {
					Vector3 push_away = normalize(collisionpoint - cannonpos);
					push_away.y = 0;
					buildables[i].model.translate(-5 * push_away);
				}
			}
		}
	}
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