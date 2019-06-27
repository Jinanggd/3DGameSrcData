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
	Matrix44 R;
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
		this->mesh = Mesh::Get("data/Cannon3.OBJ");
		this->mat.shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
		this->mat.texture = Texture::Get("data/cannon3.tga");
		this->mesh->ComputeAABB();
		center = 0.5f* (this->mesh->aabb_max - this->mesh->aabb_min);
		center.y = this->mesh->aabb_min.y;
		this->tag = "PropCannon";
		this->mesh2 = new Mesh();
		this->mesh2->createQuad(0, 0, 10, 10, false);
		this->mat.texture2 = Texture::Get("data/Selection.tga");
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
	case mat_types::explosion:

		this->mesh = new Mesh();
		this->mesh->createQuad(0, 0, 20, 20, true);

		

		R.setRotation(90.0f*DEG2RAD, Vector3(0, 1, 0));

		this->mesh2 = new Mesh();

		this->mesh2->uvs = this->mesh->uvs;

		for (int i = 0; i < this->mesh->vertices.size(); i++) {

			this->mesh2->vertices.push_back(R*this->mesh->vertices[i]);
			this->mesh2->normals.push_back(R*this->mesh->normals[i]);

		}


		this->mat.shader = Shader::Get("data/shaders/basic.vs", "data/shaders/explosion.fs");
		this->mat.texture = Texture::Get("data/Explosion.tga");
		this->mat.texture2 = Texture::Get("data/Explosion.tga");

		break;

	case mat_types::buildable:
		
		//We are gonna build ourself the vertices with 
		float w = 20.0f, h = 30.0f;
		this->mesh = Mesh::Get("data/unityexport.OBJ");
		this->mat.shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
		this->mat.texture = Texture::Get("data/bricks_albedo.tga");
		//this->mesh->ComputeAABB();
		//center = 0.5f* (this->mesh->aabb_max - this->mesh->aabb_min);
		//center.y = this->mesh->aabb_min.y;

		//this->mesh2 = new Mesh();
		//this->mesh2->createQuad(center.x, center.y,
		//	this->mesh->aabb_max.x - this->mesh->aabb_min.x, this->mesh->aabb_max.z - this->mesh->aabb_min.z, false);
		g = GUI(Vector2(0, 0), Vector2(0, 0), true, GUI_Types::basic);
		this->mesh2 = new Mesh();
		this->mesh2->createQuad(0, 0, 10, 10, false);
		this->mat.texture2 = Texture::Get("data/Selection.tga");
		
		break;
	}
	
}



void EntityMesh::render() {

	if (!(type == (int)mat_types::tree) && !(type == (int)mat_types::buildable) &&!(type==(int)mat_types::explosion)) {
	

		this->mat.shader->setUniform("u_color", Vector4(1, 1, 1, 1));
		this->mat.shader->setUniform("u_texture", this->mat.texture);
		this->mat.shader->setUniform("u_model", model);
		this->mesh->render(GL_TRIANGLES);
	
	}
	else if (type == (int)mat_types::buildable) {

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);
		this->mat.shader->setUniform("u_color", Vector4(1, 1, 1, 0.5));
		this->mat.shader->setUniform("u_model", model);
		this->mat.shader->setUniform("u_texture", this->mat.texture);
		this->mesh->render(GL_TRIANGLES);
		glDisable(GL_BLEND);
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

void EntityMesh::downgrade()
{
	type = (int)mat_types::buildable;
	mat.texture = Texture::Get("data/bricks_albedo.tga");

}

bool EntityMesh::ExtractLife()
{
	life--;
	if (life == 0) return true;
	return false;
}



void EntityMesh::setPosition(Vector3 pos) {

	model.setIdentity();
	model.translate(pos.x,pos.y,pos.z);


}

Vector3 EntityMesh::getPosition() {

	return model.getTranslation();


}

bool EntityMesh::upgrade(mat_types t,float time)
{
	if (initial_time > 0) {
		float diff = time - initial_time;
		if (diff > duration) {
			initial_time = -1;
			type = (int)t;
			(int)t == 11 ? mat.texture = Texture::Get("data/Tower1.tga") : mat.texture = Texture::Get("data/Tower2.tga");
			life = 3;
			return true;
		}
	}
	return false;
}

std::vector<Vector2> EntityMesh::buildQuadUVS(float minX, float maxX, float minY, float maxY)
{
	std::vector<Vector2> uvs;
	//this->mesh->uvs.push_back(Vector2(0.25f, 0.25f));
//this->mesh->uvs.push_back(Vector2(0.0f, 0.5f));
//this->mesh->uvs.push_back(Vector2(0.25f, 0.5f));

//this->mesh->uvs.push_back(Vector2(0.0f, 0.25f));
//this->mesh->uvs.push_back(Vector2(0.0f, 0.5f));
//this->mesh->uvs.push_back(Vector2(0.25f, 0.25f));
	uvs.push_back(Vector2(maxX, minY));
	uvs.push_back(Vector2(minX, maxY));
	uvs.push_back(Vector2(maxX, maxY));

	uvs.push_back(Vector2(minX, minY));
	uvs.push_back(Vector2(minX, maxY));
	uvs.push_back(Vector2(maxX, minY));
	return uvs;
}

void EntityMesh::updateQUAD()
{
}
