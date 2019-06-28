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
	this->yawCannon = 0.0f;
	this->pitchCannon = 0.0f;
	this->speed = 0.0f;
	this->pitch = -5.1f;


	updateMatrix();
	this->camera = new Camera();
	//this->camera->lookAt(Vector3(current_position.x, current_position.y + 40, current_position.z +50),current_position, Vector3(0.f, 1.f, 0.f)); //position the camera and point to 0,0,0
	this->camera->setPerspective(70.f, 800.0f / (float)600.0f, 0.1f, 10000.f);
	this->scope = GUI(Vector2(0, 0), Vector2(0, 0), false, GUI_Types::scope);
	this->staminaBar = GUI(Vector2(10, 50), Vector2(0, 0), false, GUI_Types::basic);
	//updateCamera(Vector3(0, 10, -20));

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

		skeleton.computeFinalBoneMatrices(bone_matrices, mesh);
		this->mat.shader->setUniform("u_bones", bone_matrices);

	}

	this->mesh->render(GL_TRIANGLES);
	//this->mesh->renderAnimated(GL_TRIANGLES, this->anim);
}

void EntityPlayer::render(Camera * cam)
{
	
}

void EntityPlayer::update(float dt, std::vector<EntityMesh> props, std::vector<EntityMesh> bc,std::vector<EntityMesh> b)
{
	if(!isdead)
		playerMovement(dt, props,bc,b);
	
}

void EntityPlayer::playerMovement(float dt, std::vector<EntityMesh> props, std::vector<EntityMesh> bc,std::vector<EntityMesh>b)

{
	// Crear la matriz de rotatcion con la rotacion actual,
	// Crear el movimiento, sumarla a la posicion y multiplcarla por la matriz
	// 

	if (isoncannon) {
		Matrix44 R;
		Vector3 frontCamera = this->camera->center - this->camera->eye;
		frontCamera.normalize();
		Vector3 rightCamera = frontCamera.cross(this->camera->up);
		rightCamera.normalize();
		if (Input::isKeyPressed(SDL_SCANCODE_A)) {
			yawCannon -= 9.0f*dt;
			yawCannon = clamp(yawCannon, -maxyawCannon,maxyawCannon);
			if (yawCannon > -maxyawCannon) {
				this->camera->rotate(-9.0f*dt*DEG2RAD, Vector3(0, 1, 0));
				rotateCannon();
			}
		}

		else if (Input::isKeyPressed(SDL_SCANCODE_D)) { 
			yawCannon += 9.0f*dt;
			yawCannon = clamp(yawCannon, -maxyawCannon, maxyawCannon);
			if (yawCannon < maxyawCannon) {
				this->camera->rotate(9.0f*dt*DEG2RAD, Vector3(0, 1, 0));

				rotateCannon();
			}

		}

		else if (Input::isKeyPressed(SDL_SCANCODE_S)) { 
			pitchCannon += 9.0f*dt;
			pitchCannon = clamp(pitchCannon, -40.0f, 40.0f);

			if (pitchCannon < 40.0f) {
				this->camera->rotate(9.0f*dt*DEG2RAD, rightCamera);
				rotateCannon();
				//std::cout << " PITCH: " << pitchCannon << std::endl;

			}

		}

		else if (Input::isKeyPressed(SDL_SCANCODE_W)){

			pitchCannon -= 9.0f*dt;
			pitchCannon = clamp(pitchCannon, -40.0f, 40.0f);
			if (pitchCannon > -40.0f) {
				this->camera->rotate(-9.0f*dt*DEG2RAD, rightCamera);
				rotateCannon();
				//std::cout <<  " PITCH: " << pitchCannon << std::endl;
			}
		}

		updateCamera(props,b);
		if (Input::wasKeyPressed(SDL_SCANCODE_Z)) {
			std::cout << "YAW: " <<yawCannon << " PITCH: " << pitchCannon <<std::endl;
		}
		
	}
	else {
		//If player is not in cannon mode, we can move the player,
		Vector3 move;

		direction = KEY_UP;

		if (Input::isKeyPressed(SDL_SCANCODE_W)) { move.z -= dt * 10; }

		if (Input::isKeyPressed(SDL_SCANCODE_S)) { move.z += dt * 10; }

		if (Input::isKeyPressed(SDL_SCANCODE_A)) { yaw -= dt * 30; direction = KEY_LEFT; }

		if (Input::isKeyPressed(SDL_SCANCODE_D)) { yaw += dt * 30; direction = KEY_RIGHT; }

		if (Input::isKeyPressed(SDL_SCANCODE_Q)) pitch -= dt * 30;

		if (Input::isKeyPressed(SDL_SCANCODE_E)) pitch += dt * 30;

		pitch = clamp(pitch, -15.0f, 12.5f);


		Matrix44 R;

		R.setRotation((yaw + 180.0f)*DEG2RAD, Vector3(0, 1, 0));

		move = R * move;

		float new_speed = iscarrying ? 4:6;

		if (Input::isKeyPressed(SDL_SCANCODE_LSHIFT) && stamina > 0) {

			new_speed +=3;
			stamina -= dt*10;
			stamina = clamp(stamina, 0, 100);
			isrunning = true;

		}
		else {
			stamina += dt*5;
			isrunning = false;

			stamina = clamp(stamina, 0, 100);
		}

		velocity = velocity + move * new_speed;


		checkCollision(props,bc, b,current_position + velocity * dt, dt);

		
		//current_position = current_position + velocity * dt;

		float friction = 1.0 / (1.0 + (dt * 4.5));

		velocity = velocity * friction;

		if (iscarrying)
			updateItem(R, Vector3(0, 7, -1));

		if (isanimated)updateAnim(dt);

		updateMatrix();
		updateCamera(props,b);
	}
	

}

void EntityPlayer::rotateCannon()
{
	//this->camera->rotate(rotation, axis);
	Matrix44 inv,R,newmodel;
	Vector3 frontCannon = Cannon.model.frontVector();
	Vector3 totarget = this->camera->center - Cannon.model.getTranslation();
	totarget.normalize();
	frontCannon.normalize();

	float angle = acos(frontCannon.dot(totarget));
	if (angle == 0) return;

	Vector3 axis = cross(totarget, frontCannon);
	inv = Cannon.model;
	inv.inverse();
	axis = inv.rotateVector(axis);
	R.setRotation(angle, axis);
	newmodel = R * Cannon.model;
	Cannon.model = newmodel;
	Game::instance->world.bullets_and_cannon[CannonID].model = newmodel;
}

//Check if there is a collision to the new position of the player, if there it is, the player will keep the same position as before moving
//Depending of the Mesh tag it will happen different kind of interactions
void EntityPlayer::checkCollision(std::vector<EntityMesh> props, std::vector<EntityMesh> bc, std::vector<EntityMesh> b, Vector3 newpos,float dt)
{
	Vector3 character_center = newpos + Vector3(0, 2, 0);

	//Collision with props
	for (int i = 0; i < props.size(); i++) {

		if (props[i].type == (int)mat_types::tree || props[i].type == (int)mat_types::house || props[i].type == (int)mat_types::tower) {
			Vector3 collisionpoint, collision_normal;

			float radius = 2;

			if (props[i].type == (int)mat_types::tree) radius = 0.3;

			if (props[i].mesh->testSphereCollision(props[i].model, character_center, radius, collisionpoint, collision_normal) == true) {
				
				Vector3 push_away = normalize(collisionpoint - character_center)*dt;
				push_away.y = 0;
				current_position = current_position - push_away;
				return;

			}
		}
	}

	//Collision with bullet and cannon
	for (int i = 0; i < bc.size(); i++) {

		Vector3 collisionpoint, collision_normal;
		Matrix44 Model = bc[i].model;
		if (bc[i].type == (int)mat_types::cannon) {
			Matrix44 SS;
			SS.setScale(0.5f, 0.5f, 0.5f);
			Model = SS * Model;
		}
		if (bc[i].mesh->testSphereCollision(Model, character_center, 1, collisionpoint, collision_normal) == true) {

			Vector3 push_away = normalize(collisionpoint - character_center)*dt;
			push_away.y = 0;
			current_position = current_position - push_away;

			//Create GUI in case of Bullet or Cannon
			switch (bc[i].type)
			{
			case (int)mat_types::cannon:
				if (iscarrying || bc[i].munition.size()>0) {
					//You can Charge the Cannon
					Game::instance->world.GUIs[5].enable = false;
					//Game::instance->world.GUIs[5].index = -1;
					Game::instance->world.GUIs[6].enable = true;
					Game::instance->world.GUIs[6].index = i;

				}
				else {
					// TEXT - You need to be carrying a bullet

				}
				break;
			case (int)mat_types::bullet:
				if (!iscarrying) {
					//You can carry the bullet
					Game::instance->world.GUIs[4].enable = true;
					Game::instance->world.GUIs[4].index = i;
				}
				break;
			default:
				break;
			}

			return;

		}
		
	}

	//Collision with  buildables
	for (int i = 0; i < b.size(); i++) {
		Vector3 collisionpoint, collision_normal;
		//Matrix44 S;
		//b[i].type == (int)mat_types::buildable ? S.setScale(10, 5, 10) : S.setIdentity();
		if (b[i].mesh->testSphereCollision( b[i].model, character_center, 1, collisionpoint, collision_normal) == true) {

			Vector3 push_away = normalize(collisionpoint - character_center)*dt;
			push_away.y = 0;
			current_position = current_position - push_away;

			if (b[i].type == (int)mat_types::buildable && b[i].initial_time <0) {
				Game::instance->world.GUIs[9].enable = true;
				Game::instance->world.GUIs[9].index = i;
				if (iscarrying)
					Game::instance->world.GUIs[5].enable = false;
			}
			else if(b[i].initial_time < 0 && b[i].type == (int)mat_types::tower1 && !iscarrying && !Game::instance->world.GUIs[4].enable){
				Game::instance->world.GUIs[10].enable = true;
				Game::instance->world.GUIs[10].index = i;
				if (iscarrying)
					Game::instance->world.GUIs[5].enable = false;
			}

			return;

		}
	}

	//No collision
	current_position = newpos;


}

void EntityPlayer::updateItem(Matrix44 r,Vector3 dir)
{
	Vector3 newposition = current_position + r * dir;
	Game::instance->world.updateBullets(CarryItem, newposition);
}

void EntityPlayer::updateCamera( std::vector<EntityMesh>props, std::vector<EntityMesh> b)
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
	if (isoncannon) {

		R_Yaw.setRotation(yawCannon*DEG2RAD, Vector3(0, 1, 0));
		cam_eye = Cannon.model.getTranslation() + R_Yaw * Vector3(0, 10, 0);
		this->camera->lookAt(cam_eye, camera->center, Vector3(0, 1, 0));
		scope.setPositionfrom3D(this->camera->center, Vector2(0.15f, 0.15f), this->camera->viewprojection_matrix);
		return;
	}
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
	//Check for collision of the camera
	for (int i = 0; i < b.size(); i++) {

		//if (b[i].tag == "EntityMesh") continue;
		if (b[i].type == (int)mat_types::buildable || b[i].type == (int)mat_types::tower1 || b[i].type == (int)mat_types::tower2) {


			Vector3 collisionpoint, collision_normal;


			if (b[i].mesh->testRayCollision(b[i].model, cam_eye, vector_eyetocenter, collisionpoint, collision_normal, length) == true) {

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

		anim = iscarrying ? Animation::Get("data/characters/characters/carrying_idle.skanim") : Animation::Get(idle_name);
		anim->assignTime(t);
		skeleton = anim->skeleton;

	}
	else if (speed < 0.8) //walk
	{
		anim = iscarrying ? Animation::Get("data/characters/characters/carrying_idle.skanim") : Animation::Get(idle_name);
		anim->assignTime(t);
		w = clamp(speed, 0, 1);

		animB = iscarrying ? Animation::Get("data/characters/characters/carrying_walking.skanim") : Animation::Get("data/characters/characters/walking.skanim");

		if (vel.z < 0)
			animB->assignTime(t);
		else
			animB->assignTime(-t);

		if (iscarrying) w *= 2;
		
		blendSkeleton(&anim->skeleton, &animB->skeleton, w, &skeleton);
	}

	

	else //run
	{

		if (iscarrying) { 
		

			anim = isrunning ? Animation::Get("data/characters/characters/carrying_running.skanim") : Animation::Get("data/characters/characters/carrying_walking.skanim");
			anim->assignTime(t);
			skeleton = anim->skeleton;


		}

		else if (isrunning) {


			w = clamp(speed - 1.0, 0, 1);
			anim = Animation::Get("data/characters/characters/running_FAST.skanim");
			if (vel.z < 0)
				anim->assignTime(t);
			else
				anim->assignTime(-t);
			animB = Animation::Get("data/characters/characters/running_FAST.skanim");
			if (vel.z < 0)
				animB->assignTime((t / anim->duration) * animB->duration);
			else
				animB->assignTime(-(t / anim->duration) * animB->duration);
			blendSkeleton(&anim->skeleton, &animB->skeleton, 2*w , &skeleton);
		
		
		}

		else {
		
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

	}

	


	skeleton.computeFinalBoneMatrices(bone_matrices, mesh);


}

void EntityPlayer::build(std::vector<EntityMesh> vector, mat_types t)
{
	Vector3 character_center = current_position + Vector3(0, 2, 0);
	Vector3 collisionpoint, collisionnormal;

	for (int i = 0; i < vector.size(); i++) {
		if (vector[i].type != (int)mat_types::buildable) continue;
		if (vector[i].mesh->testSphereCollision(vector[i].model, character_center, 5, collisionpoint, collisionnormal) == true) {
			Game::instance->world.buildables[i].tobeupgrate = t;
			Game::instance->world.buildables[i].initial_time = *Game::instance->world.time;
			Game::instance->world.GUIs[9].enable = false;
			Game::instance->world.GUIs[9].index = -1;
			if (iscarrying)
				Game::instance->world.GUIs[5].enable = true;
			return;
		}
	}
}

void EntityPlayer::grab(std::vector<EntityMesh> vector)
{
	Vector3 character_center = current_position + Vector3(0, 2, 0);
	Vector3 collisionpoint,collisionnormal;
	for (int i = 0; i < vector.size(); i++) {
		if (vector[i].mesh->testSphereCollision(vector[i].model, character_center, 2, collisionpoint, collisionnormal) == true) {
			switch (vector[i].type)
			{
			case (int)mat_types::cannon:
				if ((iscarrying&&!isoncannon) || vector[i].munition.size() >0) {
					latestposition = current_position;
					isoncannon = true;
					iscarrying = false;
					scope.enable = true;
					Cannon = vector[i];
					CannonID = i;

					if (CarryItem >= 0) {
						chargeCannon();
					}

					//position of the player
					Vector3 frontCannon = Cannon.model.frontVector().normalize();
					Vector3 frontPlayer = model.frontVector().normalize();
					//frontCannon.y = frontPlayer.y;
					frontPlayer.y = 0;
					//model.setFrontAndOrthonormalize(Vector3(frontCannon));
					
					//if (angle >= 180.0f) {
					//	yaw -= angle;
					//}
					//else {
					//	yaw += angle;
					//}
					
					
					//
					//float previousy = current_position.y;
					//current_position = Cannon.model.getTranslation()-frontCannon*15;
					//current_position.y = previousy;
					//////float angle = acos(clamp(dot(frontCannon, frontPlayer), -1.0f, 1.0f))*RAD2DEG;
					current_position.y = -100;
					updateMatrix();
					////yaw += angle;
					yawCannon = Cannon.rotation;
					maxyawCannon = yawCannon + 40.0f;
					//updateMatrix();

					//position of the Camera
					Matrix44 R_Yaw, R_Pitch;
					Vector3 right, cam_eye, cam_center, front;

					R_Yaw.setRotation(yawCannon*DEG2RAD, Vector3(0, 1, 0));
					right = R_Yaw * Vector3(1, 0, 0);
					pitchCannon = 0;

					R_Pitch.setRotation(pitchCannon*DEG2RAD, right);
					cam_eye = Cannon.model.getTranslation()  +  R_Yaw*Vector3(0, 10, 0);
					//front = Cannon.model.frontVector();
					front = R_Yaw * R_Pitch * Vector3(0, 20, 30);
					cam_center = cam_eye + front;

					this->camera->lookAt(cam_eye, cam_center, Vector3(0, 1, 0));

					//initial model matrix
					initialmatrixCannon = Cannon.model;

					//rotateCannon();

					Game::instance->world.GUIs[5].enable = false;
					Game::instance->world.GUIs[5].index = -1;
					Game::instance->world.GUIs[6].enable = false;
					Game::instance->world.GUIs[6].index = -1;
					Game::instance->world.GUIs[7].enable = true;
					Game::instance->world.GUIs[8].enable = true;
					//Game::instance->world.GUIs[7].index = Game::instance->world.GUIs[6].index;
					return;
				}
				else {
					//You cannot fire without bullet

				}
				break;
			case (int)mat_types::bullet:
				if (iscarrying) {
					//TEXT - You cannot grab more than one bullet
				}
				else {
					iscarrying = true;
					Game::instance->mysound.playSound(sound_types::pick, false);
					CarryItem = i;
					Game::instance->world.GUIs[4].enable = false;
					Game::instance->world.GUIs[5].index = Game::instance->world.GUIs[4].index;
					Game::instance->world.GUIs[5].enable = true;
					Game::instance->world.GUIs[4].index = -1;
					return;
				}
				break;
			case(int)mat_types::tower1:
				if (iscarrying) {

				}
				else {
					iscarrying = true;
					EntityMesh b = EntityMesh(mat_types::bullet);
					CarryItem = Game::instance->world.bullets_and_cannon.size();
					Game::instance->world.bullets_and_cannon.push_back(b);
					Matrix44 R;
					R.setRotation((yaw + 180.0f)*DEG2RAD, Vector3(0, 1, 0));

					updateItem(R, Vector3(0, 7, -1));
					Game::instance->world.GUIs[10].enable = false;
					Game::instance->world.GUIs[10].index = -1;
					Game::instance->world.GUIs[5].index = CarryItem;
					Game::instance->world.GUIs[5].enable = true;
				}
				break;
			default:
				break;
			}
		}
	}
	
}

void EntityPlayer::chargeCannon()
{
	
	Game::instance->world.bullets_and_cannon[CarryItem].model.translate(Vector3(0, -20, 0));
	Cannon.munition.push_back(CarryItem);
	Game::instance->world.bullets_and_cannon[CannonID].munition.push_back(CarryItem);
	//adds 4 bullets for everyone you charged
	int size = Game::instance->world.bullets_and_cannon.size();
	for (int i = 0; i < 4; i++) {
		Cannon.munition.push_back(size);
		Game::instance->world.bullets_and_cannon[CannonID].munition.push_back(size);
		EntityMesh b = EntityMesh(mat_types::bullet);
		b.model.setTranslation(0, -100, 0);
		Game::instance->world.bullets_and_cannon.push_back(b);
		size = Game::instance->world.bullets_and_cannon.size();
	}
	CarryItem = -1;
}

void EntityPlayer::throwItem()
{
	if (iscarrying) {
		Matrix44 R;
		R.setIdentity();
		R.setRotation((yaw + 180.0f)*DEG2RAD, Vector3(0, 1, 0));
		updateItem(R, Vector3(5, 1, -3));
		iscarrying = false;
		CarryItem = -1;

		Game::instance->world.GUIs[5].enable = false;
		Game::instance->world.GUIs[5].index = -1;
	}

	if (isoncannon) {
		current_position = latestposition;
		updateMatrix();
		isoncannon = false;
		
		if (CarryItem > 0) {
			Game::instance->world.bullets_and_cannon[CannonID].munition.push_back( CarryItem );
		}
		Game::instance->world.bullets_and_cannon[CannonID].model = initialmatrixCannon;
		CannonID = -1;

		Game::instance->world.GUIs[7].enable = false;
		Game::instance->world.GUIs[8].enable = false;
		scope.enable = false;
		//Game::instance->world.props[Game::instance->world.bullets_and_cannon[CannonID].index_propsvector].model = initialmatrixCannon;
	}
	
}

void EntityPlayer::shoot(float dt)
{
	if (Cannon.munition.size() > 0) {

		int shootedBulletindex = Cannon.munition[0];
		Game::instance->world.bullets_and_cannon[shootedBulletindex].model.setTranslation(Cannon.model.getTranslation());
		//Game::instance->world.bullets_and_cannon[shootedBulletindex].model.setTranslation(current_position);
		Vector3 direction = (this->camera->center - Cannon.model.getTranslation()).normalize();
		Game::instance->world.bullets_and_cannon[shootedBulletindex].Direction = direction;

		Game::instance->world.bullets_and_cannon[CannonID].munition.erase(Game::instance->world.bullets_and_cannon[CannonID].munition.begin());
		Cannon.munition.erase(Cannon.munition.begin());
		Game::instance->world.shootedBullet.push_back( shootedBulletindex ) ;
		Game::instance->world.bullets_and_cannon[shootedBulletindex].initial_time =* Game::instance->world.time;
		Game::instance->mysound.playSound(sound_types::cannon, false);

	}
}

void EntityPlayer::updateMatrix()
{
	this->model.setTranslation(current_position.x, current_position.y, current_position.z);
	this->model.scale(0.5f, 0.5f, 0.5f);
	this->model.rotate(yaw*DEG2RAD, Vector3(0, 1, 0));
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
	
}

Vector3 EntityPlayer::getLocalVector(Vector3 v)
{
	Matrix44 iV = this->model;
	if (iV.inverse() == false)
		std::cout << "Matrix Inverse error" << std::endl;
	Vector3 result = iV.rotateVector(v);
	return result;
}

