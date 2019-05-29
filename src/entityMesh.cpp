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

EntityMesh::EntityMesh( mat_types type)
{
	this->mesh = mesh;
	this->tag = "EntityMesh";
	this->name = "UndefName";
	this->type = (int)type;
	switch (type)

	{
	case mat_types::rock:
		this->mesh->createPlane(10);
		this->mat.shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
		this->mat.texture = Texture::Get("data/rocks.tga");

		break;

	case mat_types::tree:
		this->mesh = Mesh::Get("data/trees/trunk.obj");
		this->mesh2 = Mesh::Get("data/trees/leaves.obj");
		this->mat.shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
		this->mat.texture = Texture::Get("data/trees/trunk.tga");
		this->mat.texture2 = Texture::Get("data/trees/leaves_olive.tga");
		this->tag = "PropTree";

		break;

	case mat_types::house:

		this->mat.shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
		this->mat.texture = Texture::Get("data/house.tga");
		this->mesh = Mesh::Get("data/house.obj");
		this->tag = "PropHouse";
		break;

	case mat_types::tower:
		this->mesh = Mesh::Get("data/tower.obj");
		this->mat.shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
		this->mat.texture = Texture::Get("data/house.tga");
		this->tag = "PropTower";
		break;

	case mat_types::bullet:
		this->mesh = Mesh::Get("data/sphere.obj");
		this->mat.shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
		this->mat.texture = Texture::Get("data/bullet.tga");
		this->tag = "PropBullet";
		break;
		
	case mat_types::plane:

		this->mat.shader = Shader::Get("data/shaders/basic.vs", "data/shaders/heightmap.fs");
		this->mat.texture = Texture::Get("data/heightmap.tga");
		break;


	case mat_types::sky:
		this->mesh = Mesh::Get("data/sphere.obj");
		this->mat.shader = Shader::Get("data/shaders/basic.vs", "data/shaders/sky.fs");
		this->mat.texture = Texture::Get("data/skybox.tga");
		break;

	case mat_types::airplane:

		this->mesh = Mesh::Get("data/spitfire/spitfire.ASE");
		this->mat.shader = Shader::Get("data/shaders/basic.vs", "data/shaders/airplane.fs");
		this->mat.texture = Texture::Get("data/spitfire/spitfire_color_spec.tga");

		this->model.translate(0, 100, 0);
		this->model.scale(30, 30, 30);
		this->front = Vector3(0, 0, -1);

		this->camera = new Camera();
		this->camera->lookAt(Vector3(0.f, 300, 200), this->model.getTranslation()+Vector3(0,0,-60) , Vector3(0.f, 1.f, 0.f)); //position the camera and point to 0,0,0
		this->camera->setPerspective(70.f, 800.0f / (float)600.0f, 0.1f, 10000.f);

	
		break;
	}
	
	
}



void EntityMesh::render() {

	if (!(type == (int)mat_types::tree)) {
	

		this->mat.shader->setUniform("u_color", Vector4(1, 1, 1, 1));
		this->mat.shader->setUniform("u_texture", this->mat.texture);
		this->mat.shader->setUniform("u_model", model);
		this->mesh->render(GL_TRIANGLES);
	
	}

	else {
	
	
		this->mat.shader->setUniform("u_color", Vector4(1, 1, 1, 1));
		this->mat.shader->setUniform("u_texture", this->mat.texture);
		this->mat.shader->setUniform("u_model", model);
		this->mesh->render(GL_TRIANGLES);

		this->mat.shader->setUniform("u_texture", this->mat.texture2);
		this->mesh2->render(GL_TRIANGLES);

	
	}


}

void EntityMesh::update(float elapsed_time)
{
}



void EntityMesh::setPosition(Vector3 pos) {

	model.setIdentity();
	model.translate(pos.x,pos.y,pos.z);


}

Vector3 EntityMesh::getPosition() {

	return model.getTranslation();


}
