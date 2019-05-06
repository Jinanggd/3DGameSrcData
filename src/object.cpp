#include "object.h"
#include "shader.h"
#include "texture.h"
#include "camera.h"

Object::Object()
{
	this->mesh = NULL;
    this->mat.shader = NULL;
	this->mat.texture = NULL;

}


Object::~Object()
{

}

Object::Object(Mesh * mesh, mat_types type)
{
	this->mesh = mesh;

	switch (type)

	{
	case mat_types::rock:

		this->mat.shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
		this->mat.texture = Texture::Get("data/rocks.tga");

		break;

	case mat_types::tree:

		this->mat.shader = Shader::Get("data/shaders/basic.vs", "data/shaders/tree_texture.fs");
		this->mat.texture = Texture::Get("data/tree.tga");


		break;

	case mat_types::house:

		this->mat.shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
		this->mat.texture = Texture::Get("data/house.tga");

		break;

	case mat_types::plane:

		this->mat.shader = Shader::Get("data/shaders/basic.vs", "data/shaders/heightmap.fs");
		this->mat.texture = Texture::Get("data/heightmap.tga");

		break;

	}



}

void Object::render() {


	this->mat.shader->setUniform("u_color", Vector4(1, 1, 1, 1));
	this->mat.shader->setUniform("u_texture", this->mat.texture);
	this->mat.shader->setUniform("u_model", m);
	this->mesh->render(GL_TRIANGLES);

}

void Object::setPosition(Vector3 pos) {

	m.setIdentity();
	m.translate(pos.x,pos.y,pos.z);


}
