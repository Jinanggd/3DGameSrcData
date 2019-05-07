#include "entityMesh.h"
#include "shader.h"
#include "texture.h"
#include "camera.h"

EntityMesh::EntityMesh() : Entity()
{
	this->mesh = NULL;
	this->lowmesh = NULL;
    this->mat.shader = NULL;
	this->mat.texture = NULL;

}


EntityMesh::~EntityMesh()
{

}

EntityMesh::EntityMesh(Mesh * mesh, mat_types type)
{
	this->mesh = mesh;
	this->tag = "EntityMesh";
	this->name = "UndefName";
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



	case mat_types::sky:

		this->mat.shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
		this->mat.texture = Texture::Get("data/skybox.tga");
		break;


	}


}

void EntityMesh::render() {


	this->mat.shader->setUniform("u_color", Vector4(1, 1, 1, 1));
	this->mat.shader->setUniform("u_texture", this->mat.texture);
	this->mat.shader->setUniform("u_model", model);
	this->mesh->render(GL_TRIANGLES);

}

void EntityMesh::setPosition(Vector3 pos) {

	model.setIdentity();
	model.translate(pos.x,pos.y,pos.z);


}

Vector3 EntityMesh::getPosition() {

	return model.getTranslation();


}
