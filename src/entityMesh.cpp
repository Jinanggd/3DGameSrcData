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
		
	case mat_types::cannon:
		this->mesh = Mesh::Get("data/Cannon.obj");
		this->mat.shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
		this->mat.texture = Texture::Get("data/Cannon.tga");
		this->tag = "PropCannon";
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

	case mat_types::buildable:
		
		//We are gonna build ourself the vertices with 

		float w = 20.0f, h = 100.0f;
		this->mesh->vertices.clear();
		this->mesh->uvs.clear();
		this->mesh->normals.clear();

		//vertices.push_back(Vector3(size, 0, size));
		//vertices.push_back(Vector3(size, 0, -size));
		//vertices.push_back(Vector3(-size, 0, -size));
		//vertices.push_back(Vector3(-size, 0, size));
		//vertices.push_back(Vector3(size, 0, size));
		//vertices.push_back(Vector3(-size, 0, -size));

		//****5,2-------------3********
		//****4---------------1,6*******


		//We just build the pointsby hand
		{


			this->mesh->vertices.push_back(Vector3(w, 0, 0));
			this->mesh->vertices.push_back(Vector3(0, h, 0));
			this->mesh->vertices.push_back(Vector3(w, h, 0));

			this->mesh->vertices.push_back(Vector3(0, 0, 0));
			this->mesh->vertices.push_back(Vector3(0, h, 0));
			this->mesh->vertices.push_back(Vector3(w, 0, 0));

			this->mesh->vertices.push_back(Vector3(w, 0, w));
			this->mesh->vertices.push_back(Vector3(w, h, 0));
			this->mesh->vertices.push_back(Vector3(w, h, w));

			this->mesh->vertices.push_back(Vector3(w, 0, 0));
			this->mesh->vertices.push_back(Vector3(w, h, 0));
			this->mesh->vertices.push_back(Vector3(w, 0, w));

			this->mesh->vertices.push_back(Vector3(0, 0, w));
			this->mesh->vertices.push_back(Vector3(w, h, w));
			this->mesh->vertices.push_back(Vector3(0, h, w));

			this->mesh->vertices.push_back(Vector3(w, 0, w));
			this->mesh->vertices.push_back(Vector3(w, h, w));
			this->mesh->vertices.push_back(Vector3(0, 0, w));

			this->mesh->vertices.push_back(Vector3(0, 0, 0));
			this->mesh->vertices.push_back(Vector3(0, h, w));
			this->mesh->vertices.push_back(Vector3(0, h, 0));

			this->mesh->vertices.push_back(Vector3(0, 0, w));
			this->mesh->vertices.push_back(Vector3(0, h, w));
			this->mesh->vertices.push_back(Vector3(0, 0, 0));

			//UVS
			this->mesh->uvs.push_back(Vector2(1, 0));
			this->mesh->uvs.push_back(Vector2(0, 1));
			this->mesh->uvs.push_back(Vector2(1, 1));

			this->mesh->uvs.push_back(Vector2(0, 0));
			this->mesh->uvs.push_back(Vector2(0, 1));
			this->mesh->uvs.push_back(Vector2(1, 0));

			this->mesh->uvs.push_back(Vector2(1, 0));
			this->mesh->uvs.push_back(Vector2(0, 1));
			this->mesh->uvs.push_back(Vector2(1, 1));

			this->mesh->uvs.push_back(Vector2(0, 0));
			this->mesh->uvs.push_back(Vector2(0, 1));
			this->mesh->uvs.push_back(Vector2(1, 0));

			this->mesh->uvs.push_back(Vector2(1, 0));
			this->mesh->uvs.push_back(Vector2(0, 1));
			this->mesh->uvs.push_back(Vector2(1, 1));

			this->mesh->uvs.push_back(Vector2(0, 0));
			this->mesh->uvs.push_back(Vector2(0, 1));
			this->mesh->uvs.push_back(Vector2(1, 0));

			this->mesh->uvs.push_back(Vector2(1, 0));
			this->mesh->uvs.push_back(Vector2(0, 1));
			this->mesh->uvs.push_back(Vector2(1, 1));

			this->mesh->uvs.push_back(Vector2(0, 0));
			this->mesh->uvs.push_back(Vector2(0, 1));
			this->mesh->uvs.push_back(Vector2(1, 0));
		}


		this->mesh = Mesh::Get("data/box.ASE");
		//this->mat.shader = Shader::Get("data/shaders/basic.vs", "data/shaders/flat.fs");
		break;
	}
	
}



void EntityMesh::render() {

	if (!(type == (int)mat_types::tree) && !(type == (int)mat_types::buildable)) {
	

		this->mat.shader->setUniform("u_color", Vector4(1, 1, 1, 1));
		this->mat.shader->setUniform("u_texture", this->mat.texture);
		this->mat.shader->setUniform("u_model", model);
		this->mesh->render(GL_TRIANGLES);
	
	}
	else if (type == (int)mat_types::buildable) {
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);
		this->mat.shader->setUniform("u_color", Vector4(0, 0, 1, 0.1f));
		this->mat.shader->setUniform("u_model", model);
		this->mesh->render(GL_TRIANGLES);
		//glDisable(GL_BLEND);
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
