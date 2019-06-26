#include "EntityAI.h"
#include "shader.h"
#include "texture.h"
#include "camera.h"
#include "input.h"
#include "game.h"
#include <cmath>     
#include "..\..\TJE_framework_2019\src\entityAI.h"




EntityAI::EntityAI() : Entity()
{

	mesh = Mesh::Get("data/characters/characters/male.mesh");
	mat.shader = isanimated ? Shader::Get("data/shaders/skinning.vs", "data/shaders/texture.fs") : Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
	mesh->box.halfsize *= 2;
	mat.texture = Texture::Get("data/characters/characters/titan.png");
	anim = Animation::Get("data/characters/characters/crouch_walking.skanim");

	this->yaw = 0.0f;
	this->speed = 2.0f;
	this->pitch = -1.1f;
	animtime = 0;

	updateMatrix();
	this->camera = new Camera();
	camera->lookAt(Vector3(0.f, 100.f, 100.f), Vector3(0.f, 0.f, 0.f), Vector3(0.f, 1.f, 0.f)); //position the camera and point to 0,0,0
	this->camera->setPerspective(70.f, 800.0f / (float)600.0f, 0.1f, 10000.f);
	camera->enable();
	state = SEARCH;
	hpbar = GUI(Vector2(0, 0), Vector2(0, 0), true, GUI_Types::basic);


}

EntityAI::EntityAI(float *time) : EntityAI()
{

	this->time = time;
}

EntityAI::EntityAI(float *time, bool init) : EntityAI()
{

	this->time = time;

	this->anim = Animation::Get("data/characters/characters/breakdance.skanim");

	state = DANCE;
}


EntityAI::EntityAI(float *time, Matrix44 target) : EntityAI()
{
	this->target = target;
	this->time = time;
	istargetplayer = true;
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


void EntityAI::update(float dt, std::vector<EntityMesh> props, std::vector<EntityMesh>b)
{

	float t;

	if (state == HURT) {

		t = *time - animtime;
		updateAnim(t);

	}
	else if (state == DEAD) {

		t = *time - animtime;
		updateAnim(t);

	}

	else {

		if (state == IDLE) updateAnim(dt);

		if (isnear()) {

			state = ATTACK;
			if(indexBuildable >=0){
				if (Game::instance->world.buildables[indexBuildable].life < 0) {
					state = SEARCH;
					return;
				}
			}
			
			updatedirection(dt, props);
			t = *time - animtime;
			updateAnim(t);

			if (istargetplayer) {
				//Gameover
				Game::instance->isOver = true;
				Matrix44 R;
				R.setRotation(Game::instance->world.Player->yaw*DEG2RAD, Vector3(0, 1, 0));
				Vector3 pos_eye = Game::instance->world.Player->current_position + Vector3(0, 3, -5);
				Game::instance->world.Player->isdead = true;
				Vector3 pos_center = current_position + Vector3(0, 8, 0);
				Game::instance->world.camera->lookAt(pos_eye, pos_center, Vector3(0, 1, 0));

			}
			

		}

		else {

			//animtime = *time;

			state = SEARCH;

			updatedirection(dt, props);

			updateAnim(dt);
		}

	}

	checkCollision(props,b, current_position + (velocity * dt), dt);
	hpbar.setPositionfrom3D(current_position+Vector3(0,80,0), Vector2(0.3f*(1 / 3.0f*life), 0.03f),
		Game::instance->world.camera->viewprojection_matrix);


}

void EntityAI::updatedirection(float dt, std::vector<EntityMesh> props)

{

	Vector3 mytarget = target.getTranslation();
	current_position = this->model.getTranslation();

	direction = mytarget - current_position;

	if (direction.length() == 0.0f) return;

	direction.normalize();

	direction.y = 0;

	Vector3 AIfront = this->model.frontVector().normalize();

	AIfront.y = 0;

	Vector2 a, b;

	a = Vector2(direction.x, direction.z);
	b = Vector2(AIfront.x, AIfront.z);

	float angle = ComputeSignedAngle(a, b);


	velocity = velocity + direction * 3;

	speed = velocity.length() * 0.1;

	Matrix44 R;

	yaw += angle;


	if (!isnear()) {
		current_position = current_position + (dt*speed)*direction;
		this->model.setTranslation(current_position.x, current_position.y, current_position.z);
		this->model.rotate(yaw*DEG2RAD, Vector3(0, 1, 0));
		
	}

	else this->model.rotate(angle*DEG2RAD, Vector3(0, 1, 0));


	float friction = 1.0 / (1.0 + (dt * 4.5));

	velocity = velocity * friction;



}

void EntityAI::updateTarget(EntityPlayer p, std::vector<EntityMesh> build)
{
	float distance = 10000.0f;
	if (state != SEARCH)return;
	float disttoTitan = (p.current_position - current_position).length();
	if (disttoTitan < distance) {
		distance = disttoTitan;
		target = p.model;
		istargetplayer = true;
		indexBuildable = -1;
	}
	for (int i = 0; i < build.size(); i++) {
		if (build[i].type == (int)mat_types::buildable || build[i].type == (int)mat_types::tower1) continue;
		disttoTitan = (build[i].model.getTranslation() - current_position).length();
		if (disttoTitan < distance) {
			distance = disttoTitan;
			target = build[i].model;
			istargetplayer = false;
			indexBuildable = i;
		}
	}
}

//Check if there is a collision to the new position of the player, if there it is, the player will keep the same position as before moving
//Depending of the Mesh tag it will happen different kind of interactions
void EntityAI::checkCollision(std::vector<EntityMesh> props, std::vector<EntityMesh> b, Vector3 newpos, float dt)
{
	Vector3 character_center = newpos + Vector3(0, 4, 0);


	for (int i = 0; i < props.size(); i++) {

		if (props[i].type == (int)mat_types::tree || props[i].type == (int)mat_types::house || props[i].type == (int)mat_types::tower) {


			Vector3 collisionpoint, collision_normal;

			if (props[i].mesh->testSphereCollision(props[i].model, character_center, 6, collisionpoint, collision_normal) == true) {

				Vector3 push_away = normalize(character_center - collisionpoint)*dt;
				push_away.y = 0;
				current_position = current_position - push_away;



				Vector3 right = this->model.rightVector();

				right.normalize();

				float dot_result = dot(push_away.normalize(), right);

				float angle = acos(clamp(dot_result, -1, 1))*RAD2DEG;

				if (angle < 90) this->model.translate((0.2f)*(push_away + right));

				else {

					right.x = -right.x;
					right.z = -right.z;
					this->model.translate((0.2f)*(push_away + right));


				}
				return;
			}


		}
	}

	for (int i = 0; i < b.size(); i++) {

		Vector3 collisionpoint, collision_normal;

		if (b[i].mesh->testSphereCollision(b[i].model, character_center, 4, collisionpoint, collision_normal) == true) {

			Vector3 push_away = normalize(character_center - collisionpoint)*dt;
			push_away.y = 0;
			current_position = current_position - push_away;



			Vector3 right = this->model.rightVector();

			right.normalize();

			float dot_result = dot(push_away.normalize(), right);

			float angle = acos(clamp(dot_result, -1, 1))*RAD2DEG;

			if (angle < 90) this->model.translate((0.2f)*(push_away + right));

			else {

				right.x = -right.x;
				right.z = -right.z;
				this->model.translate((0.2f)*(push_away + right));


			}


			return;

		}

	}

	//current_position = newpos;


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


	Animation* anim = NULL;
	Animation* animB = NULL;
	const char* idle_name = "data/characters/characters/idle.skanim";

	if (velocity.length() > 0.01)
	{
		normalize(velocity); //vel in local space
		Matrix44 R;
		R.rotate(-yaw * DEG2RAD, Vector3(0, 1, 0));
		vel = R * vel;
	}



	switch (state)
	{

	case  EntityAI::DANCE:

		anim = Animation::Get("data/characters/characters/breakdance.skanim");
		anim->assignTime(t);
		//anim->skeleton.getBoneMatrix("mixamorig_Hips").scale(2, 2, 2);
		anim->skeleton.getBoneMatrix("mixamorig_Head").scale(2, 2, 2);
		anim->skeleton.getBoneMatrix("mixamorig_Spine2").scale(2, 2, 2);
		skeleton = anim->skeleton;


		break;


	case EntityAI::IDLE:

		anim = Animation::Get("data/characters/characters/reaction_hit.skanim");

		anim->assignTime(t);
		anim->skeleton.getBoneMatrix("mixamorig_Hips").scale(4,4,4);
		anim->skeleton.getBoneMatrix("mixamorig_Head").scale(2, 2, 2);
		anim->skeleton.getBoneMatrix("mixamorig_Spine2").scale(2, 2, 2);
		skeleton = anim->skeleton;


		break;

	case EntityAI::SEARCH:


		if (speed < 0.8) //walk

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
			skeleton.getBoneMatrix("mixamorig_Hips").scale(2, 2, 2);
			skeleton.getBoneMatrix("mixamorig_Head").scale(2, 2, 2);
			skeleton.getBoneMatrix("mixamorig_Spine2").scale(2, 2, 2);
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
			skeleton.getBoneMatrix("mixamorig_Hips").scale(2, 2, 2);
			skeleton.getBoneMatrix("mixamorig_Head").scale(2, 2, 2);
			skeleton.getBoneMatrix("mixamorig_Spine2").scale(2, 2, 2);

		}

		break;
	case EntityAI::ATTACK:


		anim = Animation::Get("data/characters/characters/attack.skanim");
		
		anim->assignTime(t);


		if (dt > anim->duration) {
			if (!istargetplayer && indexBuildable >= 0){
				//Subsctract life to building
				std::cout << state << std::endl;
				if (Game::instance->world.buildables[indexBuildable].ExtractLife()) {
					Game::instance->world.buildables[indexBuildable].downgrade();
					state = SEARCH;
					startedattack = false;
				}
			}
			
			//std::cout << "HIT " <<std::endl;
			//a++;
			animtime = *time;
		}
		anim->skeleton.getBoneMatrix("mixamorig_Hips").scale(2, 2, 2);

		anim->skeleton.getBoneMatrix("mixamorig_Head").scale(2, 2, 2);
		anim->skeleton.getBoneMatrix("mixamorig_Spine2").scale(2, 2, 2);
		skeleton = anim->skeleton;

		break;
	case EntityAI::HURT:

		anim = Animation::Get("data/characters/characters/reaction_hit.skanim");
		
		if (dt > anim->duration) state = SEARCH;
		anim->assignTime(dt,false);
		anim->skeleton.getBoneMatrix("mixamorig_Hips").scale(2, 2, 2);

		anim->skeleton.getBoneMatrix("mixamorig_Head").scale(2, 2, 2);
		anim->skeleton.getBoneMatrix("mixamorig_Spine2").scale(2, 2, 2);
		skeleton = anim->skeleton;

		break;
	case EntityAI::DEAD:
		anim = Animation::Get("data/characters/characters/dying.skanim");

		anim->assignTime(dt, false);
		anim->skeleton.getBoneMatrix("mixamorig_Hips").scale(2, 2, 2);

		anim->skeleton.getBoneMatrix("mixamorig_Head").scale(2, 2, 2);
		anim->skeleton.getBoneMatrix("mixamorig_Spine2").scale(2, 2, 2);
		skeleton = anim->skeleton;

		break;
	default:

		break;
	}



	skeleton.computeFinalBoneMatrices(bone_matrices, mesh);


}

void EntityAI::substractLife()
{
	life--;
	if (life == 0) {
		animtime = *time;
		state = DEAD;
	}
	else
		state = HURT;
}

bool EntityAI::isnear() {

	
	Vector3 mypost, mytarget;

	mypost = current_position;

	mypost.y = 0;

	mytarget = target.getTranslation();

	mytarget.y = 0;


	float distance = (mytarget - mypost).length();

	if (istargetplayer) {
		if (distance <= 15) {
			if (!startedattack) {
				startedattack = true;
				animtime = *time;
			}
			return true;
		}
	}
	else if (distance <= 30) {
		if (!startedattack) {
			startedattack = true;
			animtime = *time;
		}
		return true;
	}
	

	return false;

}


void EntityAI::updateMatrix()
{
	this->model.setTranslation(current_position.x, current_position.y, current_position.z);
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
	this->model.setTranslation(x, y, z);

}

Vector3 EntityAI::getLocalVector(Vector3 v)
{
	Matrix44 iV = this->model;
	if (iV.inverse() == false)
		std::cout << "Matrix Inverse error" << std::endl;
	Vector3 result = iV.rotateVector(v);
	return result;
}