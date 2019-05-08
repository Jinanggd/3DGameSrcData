#include "entityCloud.h"
#include "shader.h"
#include "texture.h"
#include "camera.h"

EntityCloud::EntityCloud() : Entity()
{
	this->mat.shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
	this->mat.texture = Texture::Get("data/cloud.tga");
	this->model.translate(0, 10, 0);
	this->name = "Cloud";
	this->tag = "CLOUD";
	plane = new	Mesh();
	plane->createPlane(1024);
}


EntityCloud::~EntityCloud()
{

}

void EntityCloud::render() {


	this->mat.shader->setUniform("u_color", Vector4(1, 1, 1, 1));
	this->mat.shader->setUniform("u_texture", this->mat.texture);
	this->mat.shader->setUniform("u_model", model);
	this->plane->render(GL_TRIANGLES);
	

}

void EntityCloud::render(Camera * cam)
{
	
	this->mat.shader->setUniform("u_color", Vector4(1, 1, 1, 0.7));
	this->mat.shader->setUniform("u_texture", this->mat.texture);
	this->mat.shader->setUniform("u_model", model);
	this->plane->render(GL_TRIANGLES);
}

