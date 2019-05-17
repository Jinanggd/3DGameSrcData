#include "entityPlayer.h"
#include "shader.h"
#include "texture.h"
#include "camera.h"

EntityPlayer::EntityPlayer() : Entity()
{
	mesh = Mesh::Get("data/characters/characters/male.mesh");
	mat.shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
	mat.texture = Texture::Get("data/characters/characters/male.png");

	anim = Animation::Get("data/characters/characters/running.skanim");
	anim->assignTime(10);
	anim->skeleton.computeFinalBoneMatrices(bone_matrices, mesh);
	
	this->model.setTranslation(0, 100, 0);
	this->current_position = Vector3(0, 100, 0);
	this->model.rotate(180 * DEG2RAD, Vector3(0, 1, 0));
	this->current_YRotation = 180 * DEG2RAD;
	this->model.scale(1, 1, 1);

	this->camera = new Camera();
	this->camera->lookAt(Vector3(current_position.x, current_position.y + 40, current_position.z +5),current_position, Vector3(0.f, 1.f, 0.f)); //position the camera and point to 0,0,0
	this->camera->setPerspective(70.f, 800.0f / (float)600.0f, 0.1f, 10000.f);
}


EntityPlayer::~EntityPlayer()
{

}

void EntityPlayer::render() {

	this->mat.shader->setUniform("u_color", Vector4(1, 1, 1, 1));
	this->mat.shader->setUniform("u_texture", this->mat.texture);
	this->mat.shader->setUniform("u_model", this->model);
	//this->mat.shader->setMatrix44("u_bones", bone_matrices[0]); // wadafa
	this->mesh->render(GL_TRIANGLES);
	

}

void EntityPlayer::render(Camera * cam)
{
	
}

void EntityPlayer::move(Vector3 delta)
{
	Vector3 localDelta = this->getLocalVector(delta);
	current_position = current_position - localDelta;

	//this->model = this->model*v4localDelta;
	//this->model.setTranslation(current_position.x, current_position.y, current_position.z); // Esto si funciona pero pierde las propiedades anteriores
	//this->model.translate(current_position.x, current_position.y, current_position.z); Esto no funciona

	this->model.m[12] = current_position.x;
	this->model.m[13] = current_position.y;
	this->model.m[14] = current_position.z;

	this->camera->lookAt(Vector3(current_position.x, current_position.y + 40, current_position.z + 5), current_position, Vector3(0, 1, 0));
}

void EntityPlayer::rotate(float angle, Vector3 axis)
{
	current_YRotation = current_YRotation + angle * DEG2RAD;

	this->model.rotate(current_YRotation, axis);

}

Vector3 EntityPlayer::getLocalVector(Vector3 v)
{
	Matrix44 iV = this->model;
	if (iV.inverse() == false)
		std::cout << "Matrix Inverse error" << std::endl;
	Vector3 result = iV.rotateVector(v);
	return result;
}

