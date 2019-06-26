#include "game.h"
#include "utils.h"
#include "mesh.h"
#include "texture.h"
#include "fbo.h"
#include "shader.h"
#include "input.h"
#include "entityMesh.h"
#include "world.h"
#include "rendertotexture.h"
#include <random>
#include <iostream>
#include <cmath>
#include <vector>
#include "GUI.h"

//some globals

RenderToTexture *rt, *rt_map = NULL;

float angle = 0;

Game* Game::instance = NULL;
Shader* fbo_shader = NULL;



bool ThirdCameraMode = TRUE;
int instructions = 3;


Game::Game(int window_width, int window_height, SDL_Window* window)
{
	this->window_width = window_width;
	this->window_height = window_height;
	this->window = window;
	instance = this;
	must_exit = false;
	Start = GUI(Vector2(800 / 2, 600 / 2), Vector2(800, 600), true, GUI_Types::title);
	LoadingBar = GUI(Vector2(800 / 2, 0), Vector2(0, 0), true, GUI_Types::basic);
	EndGame = GUI(Vector2(800 / 2, 600 / 2), Vector2(800, 150), true, GUI_Types::BulletKeysC);
	EndGame.buildQuadUVS(801.0f / 1600.0f, 1599.0f / 1600.0f, (1200.0f - 168.0f) / 1200.0f, (1200.0f - 79.0f) / 1200.0f);
	ClearGame = GUI(Vector2(800 / 2, 600 / 2), Vector2(800, 150), true, GUI_Types::BulletKeysC);
	ClearGame.buildQuadUVS(801.0f / 1600.0f, 1599.0f / 1600.0f, (1200.0f - 446.0f) / 1200.0f, (1200.0f - 343.0f) / 1200.0f);
	 
	fps = 0;
	frame = 0;
	time = 0.0f;
	elapsed_time = 0.0f;
	mouse_locked = false;

	//OpenGL flags
	glEnable( GL_CULL_FACE ); //render both sides of every triangle
	glEnable( GL_DEPTH_TEST ); //check the occlusions using the Z buffer

	
	//create our camera
	camera = new Camera();
	camera->lookAt(Vector3(0.f,100.f, 100.f),Vector3(0.f,0.f,0.f), Vector3(0.f,1.f,0.f)); //position the camera and point to 0,0,0
	camera->setPerspective(70.f,window_width/(float)window_height,0.1f,10000.f); //set the projection, we want to be perspective
	camera->enable();
	this->camera2D = new Camera();
	this->camera2D->setOrthographic(0, 800, 0, 600, -1, 1);

	//world = World(camera, &time);

	mysound = Sound();

	//hide the cursor
	SDL_ShowCursor(!mouse_locked); //hide or show the mouse

}

//what to do when the image has to be draw
void Game::render(void)
{
	//mysound.playSound(sound_types::background);
	mysound.playSound(sound_types::footstep);
	//mysound.playSound(sound_types::death);
	//set the clear color (the background color)
	glClearColor(1, 1, 1, 1.0);
	//glClearColor(0, 0, 0, 1.0);

	// Clear the window and the depth buffer

	if (!rt) //creamos el RT
	{
		rt = new RenderToTexture();
		rt->create(window_width, window_height, true);

		rt_map = new RenderToTexture();
		rt_map->create(200, 200, true);

	}

	rt->enable();

	glEnable(GL_DEPTH_TEST);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	if (isReady)
		renderWorld();
	else if (isLoading) 
		renderLoading();
	else{
		renderInit();
	
	}
		
	//renderWorld();

	rt->disable();


	//rt_map->enable(400, 600, 200, 200);
	//rt_map->disable();


	glDisable(GL_DEPTH_TEST);

	//if(world.Titan-) 
	//fbo_shader = isOver ? Shader::getDefaultShader("color") : Shader::getDefaultShader("screen");



	if (isOver) {
		world.cleared ? rt->toViewport(fbo_shader, Vector4(0, 0.6, 0, 1)) : rt->toViewport(fbo_shader, Vector4(1, 0, 0, 1));
		renderResult();

	}
	else
		rt->toViewport();

	//render the FPS, Draw Calls, etc

	//swap between front buffer and back buffer
	SDL_GL_SwapWindow(this->window);
}

void Game::update(double seconds_elapsed)
{
	float speed = seconds_elapsed * 50; //the speed is defined by the seconds_elapsed so it goes constant

	//example
	angle += (float)seconds_elapsed * 10.0f;

	//mouse input to rotate the cam
	if ((Input::mouse_state & SDL_BUTTON_LEFT) || mouse_locked  ) //is left button pressed?
	{
		if (ThirdCameraMode) {
			camera->rotate(Input::mouse_delta.x * 0.005f, Vector3(0.0f, -1.0f, 0.0f));
			camera->rotate(Input::mouse_delta.y * 0.005f, camera->getLocalVector(Vector3(-1.0f, 0.0f, 0.0f)));
		}
	}

	//async input to move the camera around
	if (Input::isKeyPressed(SDL_SCANCODE_LSHIFT)) { speed *= 10; }  //move faster with left shift

	if (Input::isKeyPressed(SDL_SCANCODE_W) || Input::isKeyPressed(SDL_SCANCODE_UP)) {
		if (ThirdCameraMode) {
			camera->move(Vector3(0.0f, 0.0f, 1.0f) * speed);
		}
	}

	if (Input::isKeyPressed(SDL_SCANCODE_S) || Input::isKeyPressed(SDL_SCANCODE_DOWN)) {
		if (ThirdCameraMode)
			camera->move(Vector3(0.0f, 0.0f, -1.0f) * speed);

	} 
	if (Input::isKeyPressed(SDL_SCANCODE_A) || Input::isKeyPressed(SDL_SCANCODE_LEFT)) {
		if(ThirdCameraMode)
			camera->move(Vector3(1.0f, 0.0f, 0.0f) * speed);

	}
	if (Input::isKeyPressed(SDL_SCANCODE_D) || Input::isKeyPressed(SDL_SCANCODE_RIGHT)) {
		if(ThirdCameraMode)
			camera->move(Vector3(-1.0f, 0.0f, 0.0f) * speed);
	}
	if (!ThirdCameraMode) {
		world.Player->update(seconds_elapsed, world.props, world.bullets_and_cannon,world.buildables);

	}
	if (isReady) {
		world.Player->updateAnim(time);
		if(world.Titans.size()>0)
			for(int i=0; i<world.Titans.size();i++){
				if (world.Titans[i].state == EntityAI::DEAD && world.Titans[i].life != 0) {
					world.Titans[i].life = 1;
					world.Titans[i].substractLife();
					continue;
				}
				world.Titans[i].updateTarget(*world.Player, world.buildables);
				world.Titans[i].update(seconds_elapsed*speed, world.props, world.buildables);
			
			}
		//world.Titan->updateTarget(*world.Player, world.buildables);
		//world.Titan->update(seconds_elapsed*speed, world.props, world.buildables);
		world.update(elapsed_time);
	}
	
	//to navigate with the mouse fixed in the middle
	if (mouse_locked)
		Input::centerMouse(); 
		
}

//Keyboard event handler (sync input)
void Game::onKeyDown( SDL_KeyboardEvent event )
{

	EntityMesh foo;
	switch(event.keysym.sym)
	{
		case SDLK_ESCAPE: must_exit = true; break; //ESC key, kill the app
		case SDLK_F1: Shader::ReloadAll(); break; 
		case SDLK_F2: 

			
			world.load();

			std::cout << world.mygameState.a << " "<< world.mygameState.b << std::endl;
			
			
			break;
		case SDLK_F3: 
			world.mygameState.a = 100;
			world.mygameState.b = 200;

			world.save(); break;

		case SDLK_F4:

			if (ThirdCameraMode) {
				world.camera = world.Player->camera;
				ThirdCameraMode = false;
			}
			else {
				world.camera = camera;
				ThirdCameraMode = true;
			}
			break;

		case SDLK_c:
			world.printCamPos();
			break;

		case SDLK_w:


			break;

		case SDLK_s:


			break;

		case SDLK_a: 

			break;

		case SDLK_d:
				
			break;

	}
}

void Game::onKeyUp(SDL_KeyboardEvent event)
{
	float speed = elapsed_time * 100; //the speed is defined by the seconds_elapsed so it goes constant
	if(isReady)
		if (world.GUIs[11].enable) {
			world.GUIs[11].enable = false;
			return;
		}
	switch (event.keysym.sym)
	{
	case SDLK_r:
		if (!ThirdCameraMode && world.Player->isoncannon) {
			world.Player->shoot(elapsed_time*speed);
		}
		if (!ThirdCameraMode)
			world.Player->build(world.buildables, mat_types::tower1);
		break;
	case SDLK_t:
		if (!ThirdCameraMode)
			world.Player->build(world.buildables, mat_types::tower2);
		break;
	case SDLK_SPACE:
		if (!isfullyLoaded) {
			isLoading = true;
			world = World(camera, &time);
			LoadingBar.size = Vector2(100.0f, 30.0f);
			render();

			world.initGUIs();
			world.initPlayer();
			LoadingBar.size = Vector2(300.0f, 30.0f);
			LoadingBar.buildQuad();
			loaded = 40;

			render();
			world.mapinit();
			LoadingBar.size = Vector2(500.0f, 30.0f);
			LoadingBar.buildQuad();
			loaded = 75;

			render();
			world.initWorld();
			LoadingBar.size = Vector2(800.0f, 30.0f);
			LoadingBar.buildQuad();
			loaded = 100;
			render();

			isfullyLoaded = true;
			camera->lookAt(Vector3(0, 200, 0), Vector3(0, 0, 0), Vector3(0, 1, 0));
		}
		else if (instructions > -1 && isReady) {
			world.GUIs[instructions].enable = false;
			instructions--;
			if (instructions == -1) {
				world.camera = world.Player->camera;
				ThirdCameraMode = false;
				renderminimap = true;
				world.Player->staminaBar.enable = true;
				world.initSpawnTime = time;
			}
		}
		else if (isfullyLoaded) {
			isReady = true;
		}
		
		break;
	case SDLK_f:
		if (!ThirdCameraMode) {
			world.Player->grab(world.bullets_and_cannon);
			world.Player->grab(world.buildables);
		}

		break;
	case SDLK_g:
		if (!ThirdCameraMode) {
			world.Player->throwItem();
		}
		break;
	case SDLK_h:
		if (world.GUIs[11].enable == false) {
			world.GUIs[11].enable = true;
		}
		break;		
	case SDLK_F5:
		if (isOver) {
			isOver = false, isReady = false, isLoading = false, isfullyLoaded = false, renderminimap = false;
			loaded = 0;
			instructions = 3;
		}
		break;
	case SDLK_F6:
		for (int i = 0; i < world.Titans.size(); i++) {
			world.Titans[i].state = EntityAI::DEAD;
		}
		world.cleared = true;
		isOver = true;
		break;
	}
		
}
	
	


void Game::onGamepadButtonDown(SDL_JoyButtonEvent event)
{

}

void Game::onGamepadButtonUp(SDL_JoyButtonEvent event)
{

}

void Game::onMouseButtonDown( SDL_MouseButtonEvent event )
{
	if (event.button == SDL_BUTTON_MIDDLE) //middle mouse
	{
		mouse_locked = !mouse_locked;
		SDL_ShowCursor(!mouse_locked);
	}
}

void Game::onMouseButtonUp(SDL_MouseButtonEvent event)
{
}

void Game::onResize(int width, int height)
{
    std::cout << "window resized: " << width << "," << height << std::endl;
	glViewport( 0,0, width, height );
	camera->aspect =  width / (float)height;
	window_width = width;
	window_height = height;
}

void Game::renderWorld()
{
	Shader* current_shader = world.current_shader;

	glViewport(0, 0, window_width, window_height);

	if (current_shader)
	{


		//set flags
		glDisable(GL_BLEND);

		glDisable(GL_DEPTH_TEST);

		glDisable(GL_CULL_FACE);


		world.renderSkybox();

		//Double ViewPort


		glEnable(GL_DEPTH_TEST);
		world.renderplane();
		world.renderentities();


		world.renderBlendings();


		world.renderGUI();
		//world.water.render();




	}

	drawText(2, 2, getGPUStats(), Vector3(1, 1, 1), 2);

	if (renderminimap) {
		current_shader = world.map.shader;


		glViewport(window_width - 200, window_height - 200, 200, 200);

		world.rendermap();

	}
	


}

void Game::renderInit()
{
	

	Shader* current_shader;
	glViewport(0, 0, window_width, window_height);




	glDisable(GL_DEPTH_TEST);

	glDisable(GL_BLEND);

	glDisable(GL_CULL_FACE);

	current_shader = Start.shader;

	current_shader->enable();
	current_shader->setUniform("u_viewprojection", camera2D->viewprojection_matrix);
	current_shader->setUniform("u_time", time);
	Start.render();

	current_shader->disable();
	

	glEnable(GL_CULL_FACE);
	std::vector<EntityAI> titans;
	for (int i = 0; i < 6; ++i) {

		EntityAI a = EntityAI(&time, true);
		titans.push_back(a);

	}


	current_shader = Shader::Get("data/shaders/skinning.vs", "data/shaders/texture.fs");
	//current_shader = titans[0].mat.shader;
	Vector3 eye = titans[0].model.getTranslation()+titans[0].model.frontVector()*50;
	camera->lookAt(eye, titans[0].model.getTranslation()+Vector3(25,30,0), Vector3(0, 1, 0));
	for (int i = 0; i < 1; i++) {

		current_shader->enable();
		titans[i].updateAnim(time);
		//titans[i].model.setTranslation(camera->eye.x , camera->eye.y , camera->eye.z);
		//titans[i].model.setTranslation(camera->eye.x, camera->center.y+10, camera->eye.z);
		current_shader->setUniform("u_viewprojection", camera->viewprojection_matrix);
		current_shader->setUniform("u_time", time);
		titans[i].render();

	}

	current_shader->disable();


	
}

void Game::renderLoading()
{

	Shader* current_shader = LoadingBar.shader;
	glViewport(0, 0, window_width, window_height);

	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);



	glEnable(GL_DEPTH_TEST);

	glDisable(GL_CULL_FACE);


	current_shader = LoadingBar.shader;
	current_shader->enable();
	current_shader->setUniform("u_viewprojection", camera2D->viewprojection_matrix);
	current_shader->setUniform("u_time", time);
	LoadingBar.render();
	current_shader->disable();

	//glDisable(GL_BLEND);
	current_shader = Start.shader;

	current_shader->enable();
	current_shader->setUniform("u_viewprojection", camera2D->viewprojection_matrix);
	current_shader->setUniform("u_time", time);
	//LoadingBar.render();
	Start.render();
	//LoadingBar.render();
	current_shader->disable();



	isfullyLoaded ? drawText(5, 550, "Game was fully Loaded! Press SPACE to continue", Vector3(0, 0, 0), 2) : 
		drawText(5, 550, "Loading " + std::to_string(loaded) + "%", Vector3(0, 0, 0), 2);
	
	
	//LoadingBar.render();

	//current_shader->disable();
	//glEnable(GL_DEPTH_TEST);
	

}

void Game::renderResult()
{

	Shader* current_shader = ClearGame.shader;
	current_shader->enable();
	glDisable(GL_DEPTH_TEST);

	glDisable(GL_CULL_FACE);
	current_shader->setUniform("u_viewprojection", camera2D->viewprojection_matrix);
	current_shader->setUniform("u_time", time);
	if (world.cleared) {
		//Game Cleared
		
		
		ClearGame.render();

		
	}
	else{
		//Game over
		EndGame.render();
	}
	current_shader->disable();


	glEnable(GL_CULL_FACE);
}

