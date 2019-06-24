#include "world.h"
#include <fstream>
#include <iostream>
#include <algorithm>


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
	this->camera2D = new Camera();
	this->camera2D->setOrthographic(0, 800, 0, 600, -1, 1);
	this->time = time;

	//Load all the GUIs
	initGUIs();

	Player = new EntityPlayer(time);

	for (int i = 0; i < 10; i++) {

		EntityPlayer p = EntityPlayer(time);
		Players.push_back(p);

	}

	Titan = new EntityAI(time, &Player->model);
	initWorld();

	explosion = new EntityMesh(mat_types::explosion);
	explosion->model.setTranslation(0, -1000, 0);


	mapinit();
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


		current_shader = props[i].mat.shader;

		current_shader->enable();

		current_shader->setUniform("u_viewprojection", camera_matrix);

		current_shader->setUniform("u_time", *time);

		props[i].render();

		current_shader->disable();
		

	}	



	current_shader = Shader::getDefaultShader("flat");

	map.renderEntity(current_shader, Vector4(1, 1, 1, 1), Player);

	map.renderEntity(current_shader, Vector4(1,0,0,1), Titan);

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

	Matrix44 m;

	//PROPS
	for (int i = 0; i < props.size(); ++i) {
		m.setTranslation(props[i].model.getTranslation().x, props[i].model.getTranslation().y, props[i].model.getTranslation().z);
		Vector3 world_center = m*props[i].mesh->box.center;
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
				
				m.setTranslation(bullets_and_cannon[i].model.getTranslation()+Vector3(-6,2,4));
				m.rotate(90 * DEG2RAD, Vector3(1, 0, 0));
				m.scale(5, 5, 5);
				//m.rotate(sin(*time), Vector3(0, 1, 0));
				current_shader->setUniform("u_model",m);
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

	//BUILDABLES
	for (int i = 0; i < buildables.size(); ++i) {
		if (buildables[i].type == (int)mat_types::buildable)continue;
		m.setTranslation(buildables[i].model.getTranslation().x, buildables[i].model.getTranslation().y, buildables[i].model.getTranslation().z);
		Vector3 world_center = m * buildables[i].mesh->box.center;
		if (!(this->camera->testSphereInFrustum(world_center, 50) == CLIP_OUTSIDE))
		{

			current_shader = buildables[i].mat.shader;

			current_shader->enable();

			current_shader->setUniform("u_viewprojection", camera->viewprojection_matrix);

			current_shader->setUniform("u_time", *time);

			buildables[i].render();

			current_shader->disable();

		}
	}

	//TITANS
	m.setTranslation(Titan->model.getTranslation());
	Vector3 wc = m * Titan->mesh->box.center;
	if (!(this->camera->testSphereInFrustum(wc, 50) == CLIP_OUTSIDE)) {
		current_shader = Titan->mat.shader;
		current_shader->enable();
		current_shader->setUniform("u_viewprojection", camera->viewprojection_matrix);
		current_shader->setUniform("u_time", *time);
		Titan->render();
		current_shader->disable();
		if (Titan->hpbar.enable) {
			glDisable(GL_DEPTH_TEST);
			current_shader = Titan->hpbar.shader;
			current_shader->enable();
			m.setIdentity();
			current_shader->setUniform("u_viewprojection", m);
			current_shader->setUniform("u_time", *time);
			Titan->hpbar.render();
			current_shader->disable();
			glEnable(GL_DEPTH_TEST);
		}
		
	}



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
	}


	//EXPLOSION -- No need frustrum
	current_shader = explosion->mat.shader;
	current_shader->enable();
	current_shader->setUniform("u_viewprojection", camera->viewprojection_matrix);
	current_shader->setUniform("u_time", *time);
	explosion->render();
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

//Height positions
//py = mask->image.getPixel(j, i).x / 255.0f * 40.0f; HOUSES, TOWER
//py * 0.65f; BULLET
//characterpy = py * 0.63f; CHARACTER, TITAN
//--- PENDING HEIGHT
//CANNON ( also Scale )
//Buildable
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
			if (mask->image.getPixel(j, i).x >= 162 && mask->image.getPixel(j, i).x != 255 && mask->image.getPixel(j, i).z != 255) {
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

				b.model.setTranslation(px, bulletpy, pz + 20);
				b.index_propsvector = props.size();
				//props.push_back(b);
				bullets_and_cannon.push_back(b);
				
				b = EntityMesh(mat_types::bullet);
				b.model.setTranslation(px+20, bulletpy, pz + 20);
				b.index_propsvector = props.size();
				//props.push_back(b);
				bullets_and_cannon.push_back(b);

				b = EntityMesh(mat_types::bullet);
				b.model.setTranslation(px -20, bulletpy, pz + 20);
				b.index_propsvector = props.size();
				//props.push_back(b);
				bullets_and_cannon.push_back(b);

				b = EntityMesh(mat_types::cannon);

				//b.model.setTranslation(px, bulletpy, pz + 50);

				Matrix44 R,S,T;
				T.setTranslation(px, py*0.6f, pz + 50);
				S.setScale(2, 2, 2);
				//R.setRotation(90 * DEG2RAD, Vector3(0, 1, 0));
				b.model = S*R*T;

				//b.model.setUpAndOrthonormalize(Vector3(0, 1, 0));
				//b.model.rotate(90 * DEG2RAD, Vector3(0, 1, 0));
				//b.model =R* b.model;

				//b.model.translate(px, bulletpy, pz + 50);



				////b.model.scale(2,2, 2);
				//b.index_propsvector = props.size();
				////props.push_back(b);
				//bullets_and_cannon.push_back(b);
				
				this->Player->setPosition(px, characterpy, pz);


				Titan->setPosition(px - 30, characterpy, pz + 70);

				b = EntityMesh(mat_types::buildable);

				b.model.setTranslation(px+20, bulletpy, pz + 90);
				b.model.scale(5, 7, 5);
				buildables.push_back(b);

				//b = EntityMesh(mat_types::buildable);
				//b.model.setTranslation(px, bulletpy, pz+50);
				//b.model.scale(5, 7, 5);
				//buildables.push_back(b);

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

	//Bullet shooted

	if (shootedBullet > -1) {
		Vector3 currentposition = bullets_and_cannon[shootedBullet].model.getTranslation();

		if (abs(currentposition.x) > 3000 || abs(currentposition.y) > 2000 || abs(currentposition.z) > 3000) {
			// Explosion GUI
			removeBullet(shootedBullet);
			std::cout << "Too far away" << std::endl;
			return;
		}
		Vector3 newposition = currentposition + 100.0f*dt*bullets_and_cannon[shootedBullet].Direction;
		float distance = (newposition - currentposition).length();

		Vector3 collisionpoint, collisionnormal;
		//Search on propsvector

		//Hacer con test ray collision
		for (int i = 0; i < props.size(); i++) {

			//A Ray from the current point to the new position
			if (props[i].mesh->testRayCollision(props[i].model, currentposition, bullets_and_cannon[shootedBullet].Direction,
				collisionpoint, collisionnormal, distance)) {
				//Explosion GUI

				//Remove the bullet from the the vectors
				removeBullet(shootedBullet);
				std::cout << "Collision contra props" << std::endl;
				return;
			}
		}


		//Search on Titans vector
		Matrix44 SS;
		SS.scale(5, 2, 2);
		if (Titan->mesh->testRayCollision(SS*Titan->model, currentposition, bullets_and_cannon[shootedBullet].Direction,
			collisionpoint, collisionnormal, distance)){
			explosion->model.setTranslation(collisionpoint.x, collisionpoint.y, collisionpoint.z);

			//Vector3 dir = (-1.0)*bullets_and_cannon[shootedBullet].Direction;
			//dir.y = 0;

			//dir.normalize();
			//Vector3 front = Titan->model.frontVector();
			//front.y = 0;
			//front.normalize();
			//
			//Matrix44 R;
			//R.setRotation(acos(dot( dir, front )), Vector3(0, 1, 0));

			//Titan->model = R * Titan->model;

			Titan->state = EntityAI::HURT;
			Titan->animtime = *time;
			removeBullet(shootedBullet);

			return;


		}

		bullets_and_cannon[shootedBullet].model.setTranslation(newposition.x, newposition.y, newposition.z);
		//props[bullets_and_cannon[shootedBullet].index_propsvector].model.setTranslation(newposition.x, newposition.y, newposition.z);
		//props[bullets_and_cannon[index].index_propsvector].model.scale(50, 50, 50);
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
		//else if (GUIs[i].type == (int)GUI_Types::CannonKeysC) {
		//	GUIs[i].setPositionfrom3D(Player->current_position + R * Vector3(5, 13, 10), Vector2(0.2f, 0.7f),
		//		this->camera->viewprojection_matrix);
		//}
		else if (GUIs[i].type == (int)GUI_Types::Building) {
			GUIs[i].setPositionfrom3D(Player->current_position + R * Vector3(-5, 13, 0), Vector2(0.2f, 0.15f),
				this->camera->viewprojection_matrix);
		}
	}


	this->map.update();

	setAllGUItofalse();
	blendings.clear();
	//Update blendings vector
	for (int i = 0; i < buildables.size(); i++) {
		if (buildables[i].type == (int)mat_types::buildable) {
			if(!buildables[i].upgrade(buildables[i].tobeupgrate, *time))
				sortBlendingObjects(buildables[i]);

		}
	}
	

}

void World::updateBullets(int index, Vector3 position)
{

	bullets_and_cannon[index].model.setTranslation(position.x,position.y,position.z);

	//props[bullets_and_cannon[index].index_propsvector].model.setTranslation(position.x, position.y, position.z);
}

void World::removeBullet(int index)
{
	//props.erase(props.begin() + bullets_and_cannon[index].index_propsvector);
	bullets_and_cannon.erase(bullets_and_cannon.begin() + index);
	int indexatbulletvector = 0;
	//update the bullets_and_cannon vector

	for (int i = 0; i < bullets_and_cannon.size(); i++) {
		if (bullets_and_cannon[i].type == (int)mat_types::cannon) {
			if(bullets_and_cannon[i].munition.size() >0)

				for (int j = 0; j < bullets_and_cannon[i].munition.size(); j++) {
					if (bullets_and_cannon[i].munition[j] > index)

						bullets_and_cannon[i].munition[j]--;
				}
		}
	}

	if(Player->CannonID > index)
		Player->CannonID--;

	shootedBullet = -1;
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

bool World::dist( EntityMesh& lhs,  EntityMesh& rhs) {

	Vector3 position = lhs.model.getTranslation();
	float distance1 = (position - this->camera->eye).length();

	Vector3 position2 = rhs.model.getTranslation();
	float distance2 = (position2 - this->camera->eye).length();

	return distance1 >distance2;

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
		//case (int)GUI_Types::CannonKeysC:
		//	m = bullets_and_cannon[GUIs[i].index];
		//	break;
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

	//Vector3 newpoint = currentposition + 4.0f*dt*direction;
	//bullets_and_cannon[index].model.setTranslation(newpoint.x, newpoint.y, newpoint.z);
	//props[bullets_and_cannon[index].index_propsvector].model.setTranslation(newpoint.x, newpoint.y, newpoint.z);
	////props[bullets_and_cannon[index].index_propsvector].model.scale(50, 50, 50);
	
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