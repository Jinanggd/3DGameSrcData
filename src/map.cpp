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
	this->camera2D->setOrthographic(600, 800, 400, 600, -1, 1);

	//this->camera2D->setOrthographic(0, 800, 0, 600, -1, 1);

	this->time = time;
	this->player = player;

	//Load all the GUIs

	this->shader = Shader::Get("data/shaders/map.vs", "data/shaders/map.fs");

}

void Map::update() {

	Vector3 eye = player->current_position;

	eye.y += 550;

	Vector3 center = player->current_position;

	Matrix44 R;

	this->camera->lookAt(eye, center, Vector3(1, 0, 0));

	this->camera2D->lookAt(eye, center, Vector3(1, 0, 0));

}


void Map::renderEntity(Shader *current_shader, Vector4 color, EntityPlayer *Player) {

	current_shader->enable();

	Matrix44 m;

	Mesh myquad;

	myquad.createQuad(0, 1, 20, 20, true);

	m.setTranslation(Player->current_position.x, Player->current_position.y, Player->current_position.z);

	m.rotate(90 * DEG2RAD, Vector3(1, 0, 0));


	current_shader->setUniform("u_viewprojection", camera->viewprojection_matrix);

	current_shader->setUniform("u_model", m);

	current_shader->setUniform("u_color", color);



	myquad.render(GL_TRIANGLES);

	current_shader->disable();

}

void Map::renderEntity(Shader *current_shader, Vector4 color, EntityAI *AI) {

	current_shader->enable();

	Matrix44 m;

	Mesh myquad;

	myquad.createQuad(0, 1, 20, 20, true);

	m.setTranslation(AI->current_position.x, AI->current_position.y, AI->current_position.z);

	m.rotate(90 * DEG2RAD, Vector3(1, 0, 0));

	current_shader->setUniform("u_viewprojection", camera->viewprojection_matrix);

	current_shader->setUniform("u_model", m);

	current_shader->setUniform("u_color", color);

	myquad.render(GL_TRIANGLES);


	current_shader->disable();

}


void Map::renderbuildablecanon(std::vector<EntityMesh> b, std::vector<EntityMesh> c){

	Shader* current_shader = Shader::Get("data/shaders/basic_map.vs","data/shaders/basic_map.fs");
	Matrix44 SS;
	SS.setScale(1.5f, 0, 1.5f);
	for (int i = 0; i < b.size(); i++) {

		//current_shader = b[i].mat.shader;

		current_shader->enable();

		current_shader->setUniform("u_viewprojection", this->camera->viewprojection_matrix);

		current_shader->setUniform("u_time", *time);

		if (b[i].type == (int)mat_types::buildable)
			current_shader->setUniform("u_color", Vector4(1, 1, 1, 1));
		else
			current_shader->setUniform("u_color", Vector4(0, 1, 0, 1.0));

		current_shader->setUniform("u_texture", b[i].mat.texture);
		current_shader->setUniform("u_model", SS*b[i].model);
		b[i].mesh->render(GL_TRIANGLES);
		//b[i].render();

		current_shader->disable();

	}
	//for (int i = 0; i < c.size(); i++) {

	//	current_shader = c[i].mat.shader;

	//	current_shader->enable();

	//	current_shader->setUniform("u_viewprojection", this->camera->viewprojection_matrix);

	//	current_shader->setUniform("u_time", *time);

	//	current_shader->setUniform("u_color", Vector4(0, 0, 1, 1.0));

	//	current_shader->setUniform("u_texture", c[i].mat.texture);
	//	current_shader->setUniform("u_model", c[i].model);
	//	c[i].mesh->render(GL_TRIANGLES);
	//	c[i].render();

	//	current_shader->disable();

	//}
}

