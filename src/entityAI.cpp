#include "EntityAI.h"
#include "shader.h"
#include "texture.h"
#include "camera.h"
#include "input.h"
#include "game.h"
#include "pathfinders.h"
#include <cmath>     




EntityAI::EntityAI() : Entity()
{

	mesh = Mesh::Get("data/characters/characters/male.mesh");
	mat.shader = isanimated ? Shader::Get("data/shaders/skinning.vs", "data/shaders/texture.fs") : Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");

	mat.texture = Texture::Get("data/characters/characters/male.png");
	anim = Animation::Get("data/characters/characters/crouch_walking.skanim");


	this->current_position = Vector3(0, 40, 0);

	this->yaw = 0.0f;
	this->yawCannon = 0.0f;
	this->pitchCannon = -1.1f;
	this->speed = 0.0f;
	this->pitch = -1.1f;


	updateMatrix();
	this->camera = new Camera();
	camera->lookAt(Vector3(0.f, 100.f, 100.f), Vector3(0.f, 0.f, 0.f), Vector3(0.f, 1.f, 0.f)); //position the camera and point to 0,0,0
	this->camera->setPerspective(70.f, 800.0f / (float)600.0f, 0.1f, 10000.f);
	camera->enable();



}

EntityAI::EntityAI(float *time) : EntityAI()
{

	this->time = time;
}




EntityAI::~EntityAI()
{

}

void EntityAI::render() {

	this->mat.shader->setUniform("u_color", Vector4(1, 1, 1, 1));
	this->mat.shader->setUniform("u_texture", this->mat.texture);
	this->mat.shader->setUniform("u_model", this->model);

	if (isanimated) {

		anim->skeleton.getBoneMatrix("mixamorig_Head").scale(2, 2, 2);
		anim->skeleton.getBoneMatrix("mixamorig_Spine2").scale(2, 2, 2);
		skeleton.computeFinalBoneMatrices(bone_matrices, mesh);
		this->mat.shader->setUniform("u_bones", bone_matrices);
	}

	this->mesh->render(GL_TRIANGLES);
	//this->mesh->renderAnimated(GL_TRIANGLES, this->anim);
}



void EntityAI::update(float dt, std::vector<EntityMesh> props)
{
	updateAnim(dt);
	//TitanMovement(dt, props);
}

void EntityAI::TitanMovement(float dt, std::vector<EntityMesh> props)
{


	//const int start = 1;
	//const int end = 127;
	//
	//uint8* map = new uint8[128*128];

	//for (int i = 0; i < 128; ++i)
	//	for (int j = 0; j < 128; ++j)
	//		map[i + j*128] = 255;

	//int output[100];


	//int path_steps = AStarFindPathNoTieDiag(
	//	start, start, //origin (tienen que ser enteros)
	//	end, end, //target (tienen que ser enteros)
	//	map, //pointer to map data
	//	128, 128, //map width and height
	//	(int*)output, //pointer where the final path will be stored
	//	100); //max supported steps of the final path

	//if (path_steps != -1)
	//{
	//	for (int i = 0; i < path_steps; ++i)
	//		std::cout << "X: " << (output[i] % 128) << ", Y: " << floor(output[i] / 128) << std::endl;
	//}
	//
	
}





//Check if there is a collision to the new position of the player, if there it is, the player will keep the same position as before moving
//Depending of the Mesh tag it will happen different kind of interactions
void EntityAI::checkCollision(std::vector<EntityMesh> props, Vector3 newpos, float dt)
{
	Vector3 character_center = newpos + Vector3(0, 2, 0);


	for (int i = 0; i < props.size(); i++) {

		if (props[i].type == (int)mat_types::tree || props[i].type == (int)mat_types::house || props[i].type == (int)mat_types::tower ||
			props[i].type == (int)mat_types::bullet || props[i].type == (int)mat_types::cannon) {


			Vector3 collisionpoint, collision_normal;

			if (props[i].mesh->testSphereCollision(props[i].model, character_center, 2, collisionpoint, collision_normal) == true) {

				Vector3 push_away = normalize(collisionpoint - character_center)*dt;
				push_away.y = 0;
				current_position = current_position - push_away;

				return;


			}
		}
	}
	current_position = newpos;


}

void EntityAI::updateItem(Matrix44 r, Vector3 dir)
{
	Vector3 newposition = current_position + r * dir;

}

void EntityAI::updateCamera(std::vector<EntityMesh>props)
{
	Matrix44 R_Yaw;
	R_Yaw.setRotation(yaw*DEG2RAD, Vector3(0, 1, 0));

	Vector3 right = R_Yaw * Vector3(1, 0, 0);

	Matrix44 R_Pitch;
	R_Pitch.setRotation(pitch*DEG2RAD, right);
	Vector3 cam_eye = current_position + R_Yaw * Vector3(0, 10, -20);
	Vector3 front = R_Yaw * R_Pitch * Vector3(0, 5, 20);
	Vector3 cam_center = cam_eye + front;

	Vector3 vector_eyetocenter = (cam_center - cam_eye);
	float length = vector_eyetocenter.length();
	vector_eyetocenter = vector_eyetocenter.normalize();
	float maxdist = 1000000.0f;

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

void EntityAI::updateAnim(float dt) {


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

		anim->skeleton.getBoneMatrix("mixamorig_Head").scale(2, 2, 2);
		anim->skeleton.getBoneMatrix("mixamorig_Spine2").scale(2, 2, 2);
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


	skeleton.computeFinalBoneMatrices(bone_matrices, mesh);


}



void EntityAI::updateMatrix()
{
	this->model.setTranslation(current_position.x, current_position.y, current_position.z);
	this->model.scale(0.5f, 0.5f, 0.5f);
	this->model.rotate(yaw*DEG2RAD, Vector3(0, 1, 0));
}



void EntityAI::animateCharacter()
{
	isanimated = true;
	mat.shader = Shader::Get("data/shaders/skinning.vs", "data/shaders/texture.fs");
}

void EntityAI::setPosition(float x, float y, float z)
{
	this->current_position = Vector3(x, y, z);
	this->model.translate(x, y, z);
	updateMatrix();
	//updateCamera(Vector3(0, 10, -20));
}

Vector3 EntityAI::getLocalVector(Vector3 v)
{
	Matrix44 iV = this->model;
	if (iV.inverse() == false)
		std::cout << "Matrix Inverse error" << std::endl;
	Vector3 result = iV.rotateVector(v);
	return result;
}
