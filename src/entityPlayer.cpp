#include "entityPlayer.h"
#include "shader.h"
#include "texture.h"
#include "camera.h"
#include "input.h"


EntityPlayer::EntityPlayer() : Entity()
{
	mesh = Mesh::Get("data/characters/characters/male.mesh");
	mat.shader = Shader::Get("data/shaders/skinning.vs", "data/shaders/texture.fs");
	mat.texture = Texture::Get("data/characters/characters/male.png");
	anim = Animation::Get("data/characters/characters/running.skanim");

	
	this->model.setTranslation(0, 100, 0);
	this->current_position = Vector3(0, 100, 0);
	//this->model.rotate(180 * DEG2RAD, Vector3(0, 1, 0));
	//this->current_YRotation = 180 * DEG2RAD;
	this->yaw = 180.0f;
	this->speed = 0.0f;
	this->model.scale(1, 1, 1);

	this->camera = new Camera();
	this->camera->lookAt(Vector3(current_position.x, current_position.y + 40, current_position.z +50),current_position, Vector3(0.f, 1.f, 0.f)); //position the camera and point to 0,0,0
	this->camera->setPerspective(70.f, 800.0f / (float)600.0f, 0.1f, 10000.f);
}


EntityPlayer::~EntityPlayer()
{

}

void EntityPlayer::render(float time) {

	anim->assignTime(time);
	anim->skeleton.computeFinalBoneMatrices(bone_matrices, mesh);
	this->mat.shader->setUniform("u_color", Vector4(1, 1, 1, 1));
	this->mat.shader->setUniform("u_texture", this->mat.texture);
	this->mat.shader->setUniform("u_model", this->model);
	this->mat.shader->setUniform("u_bones", bone_matrices); // Imporacion (?)
	this->mesh->render(GL_TRIANGLES);
	//this->mesh->renderAnimated(GL_TRIANGLES, this->anim);
	

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

	// LA Z Tiene que ser multiplcada por la matriz de rotation
	Vector3 detras = Vector3(0, 40, 50);
	Matrix44 R;
	R.setRotation(yaw*DEG2RAD, Vector3(0, 1, 0));
	detras = R * detras; // eye de la camera

	//Falcta calcular de nuevo el center de la camra con un nuevo valor de Front a partir de la amtriz de rotacion

	this->camera->lookAt(current_position+detras, current_position, Vector3(0, 1, 0));
}



void EntityPlayer::update(float dt)
{
	// Crear la matriz de rotatcion con la rotacion actual,
	// Crear el movimiento, sumarla a la posicion y multiplcarla por la matriz
	// 
	Vector3 move;


	if (Input::isKeyPressed(SDL_SCANCODE_W) || Input::isKeyPressed(SDL_SCANCODE_UP)) { move = Vector3(0, 0, -1); speed += dt * 5; }

	if (Input::isKeyPressed(SDL_SCANCODE_S) || Input::isKeyPressed(SDL_SCANCODE_DOWN)) { move = Vector3(0, 0, 1); speed += dt * 5; }

	if (Input::isKeyPressed(SDL_SCANCODE_A) || Input::isKeyPressed(SDL_SCANCODE_LEFT)) yaw -= dt * 50;

	if (Input::isKeyPressed(SDL_SCANCODE_D) || Input::isKeyPressed(SDL_SCANCODE_RIGHT)) yaw += dt * 50;
	
	
	Matrix44 R;

	R.setRotation((yaw+180)*DEG2RAD, Vector3(0, 1, 0));

	Vector3 detras = Vector3(0, 30, 20);

	move = R * move* speed;

	current_position = current_position +  move;

	this->model.setTranslation(current_position.x, current_position.y, current_position.z);

	this->model.rotate(yaw*DEG2RAD, Vector3(0, 1, 0));

	

	this->camera->lookAt(current_position + R*detras,current_position-move, Vector3(0, 1, 0));

	speed *= 0.9;

}

Vector3 EntityPlayer::getLocalVector(Vector3 v)
{
	Matrix44 iV = this->model;
	if (iV.inverse() == false)
		std::cout << "Matrix Inverse error" << std::endl;
	Vector3 result = iV.rotateVector(v);
	return result;
}
