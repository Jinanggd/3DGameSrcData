#include "entityPlayer.h"
#include "shader.h"
#include "texture.h"
#include "camera.h"
#include "input.h"
#include "game.h"
#include <cmath>     


EntityPlayer::EntityPlayer(type mytype) : Entity()
{
	switch (mytype)
	{
	case PLAYER:
		hp = 100.0f; // Default health 
		break;
	case TITAN:
		EntityPlayer();
		//anim->skeleton.getBoneMatrix("mixamorig_Head").scale(2,2,2);
		//anim->skeleton.getBoneMatrix("mixamorig_Spine2").scale(2, 2, 2);
		break;
	default:

		EntityPlayer();
		break;
	}

}





EntityPlayer::EntityPlayer() : Entity()
{
	mesh = Mesh::Get("data/characters/characters/male.mesh");
	mat.shader = isanimated ? Shader::Get("data/shaders/skinning.vs", "data/shaders/texture.fs") : Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");

	//mat.shader = Shader::Get("data/shaders/skinning.vs", "data/shaders/texture.fs");
	mat.texture = Texture::Get("data/characters/characters/male.png");
	anim = Animation::Get("data/characters/characters/crouch_walking.skanim");

	
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
	//updateCamera(Vector3(0, 10, -20));

	actionplane.m.createPlane(10);

	
	hpbar.mat.shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
	hpbar.mat.texture = Texture::Get("data/bullet.tga");
	actionplane.mat.shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
	actionplane.mat.texture = Texture::Get("data/bullet.tga");	
	
	updateHPBar();
}

EntityPlayer::EntityPlayer(float *time) : EntityPlayer()
{

	this->time = time;
}


EntityPlayer::~EntityPlayer()
{

}

void EntityPlayer::render(float time) {

	this->mat.shader->setUniform("u_color", Vector4(1, 1, 1, 1));
	this->mat.shader->setUniform("u_texture", this->mat.texture);
	this->mat.shader->setUniform("u_model", this->model);

	if (isanimated) {
		/*
		anim->assignTime(time);
		anim->skeleton.getBoneMatrix("mixamorig_Head").scale(2, 2, 2);
		anim->skeleton.getBoneMatrix("mixamorig_Spine2").scale(2, 2, 2);
		*/
		skeleton.computeFinalBoneMatrices(bone_matrices, mesh);
		this->mat.shader->setUniform("u_bones", bone_matrices);
	}

	this->mesh->render(GL_TRIANGLES);
	//this->mesh->renderAnimated(GL_TRIANGLES, this->anim);
}

void EntityPlayer::render(Camera * cam)
{
	
}

void EntityPlayer::update(float dt, std::vector<EntityMesh> props, Mesh plane)
{
	
	// Crear la matriz de rotatcion con la rotacion actual,
	// Crear el movimiento, sumarla a la posicion y multiplcarla por la matriz
	// 
	Vector3 move;

	direction = KEY_UP;

	if (Input::isKeyPressed(SDL_SCANCODE_W) || Input::isKeyPressed(SDL_SCANCODE_UP)) { move.z -= dt * 10; }

	if (Input::isKeyPressed(SDL_SCANCODE_S) || Input::isKeyPressed(SDL_SCANCODE_DOWN)) { move.z += dt * 10; }

	if (Input::isKeyPressed(SDL_SCANCODE_A) || Input::isKeyPressed(SDL_SCANCODE_LEFT)) { yaw -= dt * 20; direction = KEY_LEFT;}

	if (Input::isKeyPressed(SDL_SCANCODE_D) || Input::isKeyPressed(SDL_SCANCODE_RIGHT)) { yaw += dt * 20; direction = KEY_RIGHT;}

	if (Input::isKeyPressed(SDL_SCANCODE_Q) ) pitch -= dt * 30;

	if (Input::isKeyPressed(SDL_SCANCODE_E) ) pitch += dt * 30;
	
	pitch = clamp(pitch, -15.0f, 12.5f);

	
	Matrix44 R;

	R.setRotation((yaw+180.0f)*DEG2RAD, Vector3(0, 1, 0));

	move = R * move;

	velocity = velocity + move*4;

	checkCollision(props, plane,current_position + velocity * dt,dt);
	//cameracheckCollision(props, dt);

	//current_position = current_position + velocity * dt;

	float friction = 1.0 / (1.0 + (dt * 4.5));

	velocity = velocity * friction;


	if (isanimated)updateAnim(dt);

	updateMatrix();
	updateCamera(props);
	updateHPBar();

	actionplane.model.setTranslation(current_position.x, current_position.y + 13, current_position.z);
	actionplane.model.scale(0.1, 0.06, 1);
	actionplane.model.rotate(90 * DEG2RAD, Vector3(1, 0, 0));
	actionplane.model.rotate(yaw*DEG2RAD, Vector3(0, 0, 1));



}

//Check if there is a collision to the new position of the player, if there it is, the player will keep the same position as before moving
//Depending of the Mesh tag it will happen different kind of interactions
void EntityPlayer::checkCollision(std::vector<EntityMesh> props, Mesh plane, Vector3 newpos,float dt)
{
	Vector3 character_center = newpos + Vector3(0, 2, 0);

	Matrix44 m;
	m.setIdentity();
	m.translate(-1024, 0, -1024);
	Vector3 plane_collision, plane_normal;

	if (plane.testRayCollision(m, character_center, Vector3(0, -1, 0), plane_collision, plane_normal) == true) {
		current_position.y = plane_collision.y;
	}

	for (int i = 0; i < props.size(); i++) {

		//if (props[i].tag == "EntityMesh") continue;
		if (props[i].type == (int)mat_types::tree || props[i].type == (int)mat_types::house || props[i].type == (int)mat_types::tower) {


			Vector3 collisionpoint, collision_normal;

			if (props[i].mesh->testSphereCollision(props[i].model, character_center, 2, collisionpoint, collision_normal) == true) {
				
				Vector3 push_away = normalize(collisionpoint - character_center)*dt;
				push_away.y = 0;
				current_position = current_position - push_away;
				
				return;

				//else if (props[i].tag == "PropBullet") {
				//	std::cout << "Bullet Detected" << std::endl;
				//	Matrix44 R_Yaw;
				//	R_Yaw.setRotation(yaw*DEG2RAD, Vector3(0, 1, 0));
				//	Vector3 right = R_Yaw * Vector3(1, 0, 0);
				//	Vector3 up = this->camera->up;

				//}

			}
		}
	}
	current_position = newpos;


}

void EntityPlayer::updateCamera( std::vector<EntityMesh>props)
{
	Matrix44 R_Yaw;
	R_Yaw.setRotation(yaw*DEG2RAD, Vector3(0, 1, 0));

	Vector3 right = R_Yaw * Vector3(1, 0, 0);

	Matrix44 R_Pitch;
	R_Pitch.setRotation(pitch*DEG2RAD, right);
	Vector3 cam_eye = current_position + R_Yaw * Vector3(0, 10, -20);
	Vector3 front = R_Yaw*R_Pitch * Vector3(0, 5, 20);
	Vector3 cam_center = cam_eye + front;

	Vector3 vector_eyetocenter = (cam_center - cam_eye);
	float length = vector_eyetocenter.length();
	vector_eyetocenter = vector_eyetocenter.normalize();
	float maxdist = 1000000.0f;
	//this->camera->lookAt(cam_eye, cam_center, Vector3(0, 1, 0));

	//Check for collision of the camera
	for (int i = 0; i < props.size(); i++) {

		//if (props[i].tag == "EntityMesh") continue;
		if (props[i].type == (int)mat_types::tree || props[i].type == (int)mat_types::house || props[i].type == (int)mat_types::tower) {


			Vector3 collisionpoint, collision_normal;


			if (props[i].mesh->testRayCollision(props[i].model, cam_eye, vector_eyetocenter, collisionpoint, collision_normal, length) == true) {

				Vector3 collisiontocenter = (cam_center - collisionpoint).normalize();
				float dist = (cam_center - collisionpoint).length();
				if (dist < maxdist) {
					maxdist = dist;
					cam_eye = collisionpoint + collisiontocenter * 3.5;

				}

			}

		}
	}

	cam_eye.x = lerp(this->camera->eye.x, cam_eye.x, 0.1);
	cam_eye.y = lerp(this->camera->eye.y, cam_eye.y, 0.1);
	cam_eye.z = lerp(this->camera->eye.z, cam_eye.z, 0.1);

	this->camera->lookAt(cam_eye, cam_center, Vector3(0, 1, 0));

}

void EntityPlayer::updateAnim(float dt) {


	//if (visibility < 0.8 && (Game::instance->frame + id) % 5)
	//	return;

	//update anim
	
	float t = *time;
	float speed = velocity.length() * 0.1;
	float w = 0.0;

	Vector3 vel = velocity;
	
	
	if (velocity.length() > 0.01)
	{
		normalize(velocity); //vel in local space
		Matrix44 R;
		R.rotate(-yaw * DEG2RAD, Vector3(0, 1, 0));
		vel = R * vel;
	}

	Animation* anim = NULL;
	Animation* animB = NULL;
	const char* idle_name = "data/characters/characters/idle.skanim";


	

	if (speed < 0.01) //idle
	{
		anim = Animation::Get(idle_name);
		anim->assignTime(t);
		skeleton = anim->skeleton;
	}
	else if (speed < 0.8) //walk
	{
		anim = Animation::Get(idle_name);
		anim->assignTime(t);
		w = clamp(speed, 0, 1);
		animB = Animation::Get("data/characters/characters/walking.skanim");
		if (vel.z < 0)
			animB->assignTime(t);
		else
			animB->assignTime(-t);
		blendSkeleton(&anim->skeleton, &animB->skeleton, w, &skeleton);
	}

	

	else //run
	{
		w = clamp(speed - 1.0, 0, 1);
		anim = Animation::Get("data/characters/characters/walking.skanim");
		if (vel.z < 0)
			anim->assignTime(t);
		else
			anim->assignTime(-t);
		animB = Animation::Get("data/characters/characters/running.skanim");
		if (vel.z < 0)
			animB->assignTime((t / anim->duration) * animB->duration);
		else
			animB->assignTime(-(t / anim->duration) * animB->duration);
		blendSkeleton(&anim->skeleton, &animB->skeleton, 0.8, &skeleton);

	}


	//if (direction == KEY_LEFT) {

	//	animB = Animation::Get("data/characters/characters/left_turn.skanim");
	//	animB->assignTime(t);

	//	float angle = abs(yaw / 360);

	//	if (fmod(animB->duration, t) < 0.5) { yaw -= 90;  std::cout << "a"; }

	//	w = clamp(angle, 0, 1);

	//	//updateCamera();
	//	updateMatrix();

	//	blendSkeleton(&skeleton, &animB->skeleton, 0.9, &skeleton);

	//}
	

	

	skeleton.computeFinalBoneMatrices(bone_matrices, mesh);


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

void EntityPlayer::updateHPBar()
{
	Matrix44 R_Yaw;
	R_Yaw.setRotation(yaw*DEG2RAD, Vector3(0, 1, 0));
	Vector3 right = camera->getLocalVector(Vector3(1, 0, 0));
	Vector3 up = Vector3(0, 1, 0);

	hpbar.m.vertices.clear();
	hpbar.m.normals.clear();
	hpbar.m.uvs.clear();
	hpbar.m.colors.clear();

	hpbar.m.vertices.push_back(50 * (right + up));
	hpbar.m.vertices.push_back(50 * (right - up));
	hpbar.m.vertices.push_back(50* ((-1.0)*right - up));
	hpbar.m.vertices.push_back(50 * ((-1.0)*right + up));
	hpbar.m.vertices.push_back(50 * (right + up) );
	hpbar.m.vertices.push_back(50 * ((-1.0)*right - up));

	hpbar.m.normals.push_back(cross(right, up));
	hpbar.m.normals.push_back(cross(right, up));
	hpbar.m.normals.push_back(cross(right, up));
	hpbar.m.normals.push_back(cross(right, up));
	hpbar.m.normals.push_back(cross(right, up));
	hpbar.m.normals.push_back(cross(right, up));


	hpbar.m.uvs.push_back(Vector2(1, 1));
	hpbar.m.uvs.push_back(Vector2(1, 0));
	hpbar.m.uvs.push_back(Vector2(0, 0));
	hpbar.m.uvs.push_back(Vector2(0, 1));
	hpbar.m.uvs.push_back(Vector2(1, 1));
	hpbar.m.uvs.push_back(Vector2(0, 0));

	

	//hpbar.m.createQuad(camera->center.x, camera->center.y, 100, 1000, false);
	hpbar.model.setIdentity();
	hpbar.model.setTranslation(camera->center.x, camera->center.y+5, camera->center.z);
	hpbar.model.scale(0.5, 0.2, 0);
	//hpbar.model.rotate(yaw * DEG2RAD, Vector3(0,1,0));

	//
	
	

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
	updateMatrix();
	//updateCamera(Vector3(0, 10, -20));
}

Vector3 EntityPlayer::getLocalVector(Vector3 v)
{
	Matrix44 iV = this->model;
	if (iV.inverse() == false)
		std::cout << "Matrix Inverse error" << std::endl;
	Vector3 result = iV.rotateVector(v);
	return result;
}

