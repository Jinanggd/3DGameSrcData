#include "entityPlayer.h"
#include "shader.h"
#include "texture.h"
#include "camera.h"
#include "input.h"


EntityPlayer::EntityPlayer() : Entity()
{
	mesh = Mesh::Get("data/characters/characters/male.mesh");
	mat.shader = isanimated ? Shader::Get("data/shaders/skinning.vs", "data/shaders/texture.fs") : Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");

	//mat.shader = Shader::Get("data/shaders/skinning.vs", "data/shaders/texture.fs");
	mat.texture = Texture::Get("data/characters/characters/male.png");
	anim = Animation::Get("data/characters/characters/running.skanim");

	
	this->current_position = Vector3(0, 40, 0);
	//this->model.rotate(180 * DEG2RAD, Vector3(0, 1, 0));
	//this->current_YRotation = 180 * DEG2RAD;
	this->yaw = 0.0f;
	this->speed = 0.0f;
	this->pitch = -1.1f;

	updateMatrix();
	this->camera = new Camera();
	//this->camera->lookAt(Vector3(current_position.x, current_position.y + 40, current_position.z +50),current_position, Vector3(0.f, 1.f, 0.f)); //position the camera and point to 0,0,0
	this->camera->setPerspective(70.f, 800.0f / (float)600.0f, 0.1f, 10000.f);
	updateCamera();
}


EntityPlayer::~EntityPlayer()
{

}

void EntityPlayer::render(float time) {

	this->mat.shader->setUniform("u_color", Vector4(1, 1, 1, 1));
	this->mat.shader->setUniform("u_texture", this->mat.texture);
	this->mat.shader->setUniform("u_model", this->model);

	if (isanimated) {
		anim->assignTime(time);
		anim->skeleton.computeFinalBoneMatrices(bone_matrices, mesh);
		this->mat.shader->setUniform("u_bones", bone_matrices);
	}
		

	this->mesh->render(GL_TRIANGLES);
	//this->mesh->renderAnimated(GL_TRIANGLES, this->anim);
}

void EntityPlayer::render(Camera * cam)
{
	
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

	if (Input::isKeyPressed(SDL_SCANCODE_Q) ) pitch -= dt * 30;

	if (Input::isKeyPressed(SDL_SCANCODE_E) ) pitch += dt * 30;
	
	pitch = clamp(pitch, -15.0f, 12.5f);

	
	Matrix44 R;
	R.setRotation((yaw+180.0f)*DEG2RAD, Vector3(0, 1, 0));
	move = R * move* speed;

	current_position = current_position +  move;

	//this->model.setTranslation(current_position.x, current_position.y, current_position.z);

	//this->model.rotate(yaw*DEG2RAD, Vector3(0, 1, 0));

	updateMatrix();
	updateCamera();


	speed *= 0.9;

}

void EntityPlayer::updateCamera()
{
	Matrix44 R_Yaw;
	R_Yaw.setRotation(yaw*DEG2RAD, Vector3(0, 1, 0));

	Vector3 right = R_Yaw * Vector3(1, 0, 0);

	Matrix44 R_Pitch;
	R_Pitch.setRotation(pitch*DEG2RAD, right);
	Vector3 cam_eye = current_position + R_Yaw *Vector3(0, 10, -20);
	Vector3 front = R_Yaw*R_Pitch * Vector3(0, 5, 20);
	Vector3 cam_center = cam_eye + front;
	this->camera->lookAt(cam_eye, cam_center, Vector3(0, 1, 0));
}

void EntityPlayer::updateMatrix()
{
	this->model.setTranslation(current_position.x, current_position.y, current_position.z);
	this->model.scale(0.5f, 0.5f, 0.5f);

	//Matrix44 R_Yaw;
	//R_Yaw.setRotation(yaw*DEG2RAD, Vector3(0, 1, 0));
	//Vector3 right = R_Yaw * Vector3(1, 0, 0);

	this->model.rotate(yaw*DEG2RAD, Vector3(0, 1, 0));
	//this->model.rotate(pitch*DEG2RAD, right);


}

void EntityPlayer::animateCharacter()
{
	isanimated = true;
	mat.shader = Shader::Get("data/shaders/skinning.vs", "data/shaders/texture.fs");
}

void EntityPlayer::setPosition(float x, float y, float z)
{
	this->current_position = Vector3(x, y, z);
	this->model.translate(x, y, z);
	updateCamera();
}

Vector3 EntityPlayer::getLocalVector(Vector3 v)
{
	Matrix44 iV = this->model;
	if (iV.inverse() == false)
		std::cout << "Matrix Inverse error" << std::endl;
	Vector3 result = iV.rotateVector(v);
	return result;
}

