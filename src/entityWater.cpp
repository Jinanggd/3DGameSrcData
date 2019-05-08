#include "entityWater.h"
#include "shader.h"
#include "texture.h"
#include "camera.h"

EntityWater::EntityWater() : Entity()
{
	this->plane = new Mesh();
	this->plane->createPlane(1024);
	this->mat.shader = Shader::Get("data/shaders/basic.vs", "data/shaders/water.fs");
	this->mat.texture = Texture::Get("data/agua.tga");
	this->model.translate(0, 0, 0);
	this->name = "Water";
	this->tag = "WATER";

}


EntityWater::~EntityWater()
{

}

void EntityWater::render() {


	this->mat.shader->setUniform("u_color", Vector4(1, 1, 1, 0.7f));
	this->mat.shader->setUniform("u_texture", this->mat.texture);
	this->mat.shader->setUniform("u_model", model);
	this->plane->render(GL_TRIANGLES);
	

}

