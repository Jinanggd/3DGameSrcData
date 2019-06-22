#include "game.h"
#include "utils.h"
#include "mesh.h"
#include "texture.h"
#include "fbo.h"
#include "shader.h"
#include "input.h"
#include "entityMesh.h"
#include "world.h"
#include <random>
#include <iostream>
#include <cmath>
#include <vector>

//some globals

float angle = 0;

Game* Game::instance = NULL;

bool ThirdCameraMode = TRUE;
int instructions = 3;


Game::Game(int window_width, int window_height, SDL_Window* window)
{
	this->window_width = window_width;
	this->window_height = window_height;
	this->window = window;
	instance = this;
	must_exit = false;

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



	//Gl viewport test  

	//glViewport(window_width / 2.0f, 0, window_width/2.0f, window_height);
	//Camera* cam = new Camera();
	//cam->lookAt(Vector3(0.f, 100.f, 100.f), Vector3(100.f, 0.f, 0.f), Vector3(0.f, 1.f, 0.f)); //position the camera and point to 0,0,0
	//cam->setPerspective(70.f, window_width / (float)window_height, 0.1f, 10000.f); //set the projection, we want to be perspective
	//cam->enable();

	world = World(camera, &time);



	//world2 = World(cam, &time);
	//world.entities.push_back(EntityMesh(Mesh::Get("data/box.ASE"), mat_types::rock));


	//hide the cursor
	SDL_ShowCursor(!mouse_locked); //hide or show the mouse

}

//what to do when the image has to be draw
void Game::render(void)
{
	//set the clear color (the background color)
	glClearColor(0.372, 0.827, 0.945, 1.0);
	//glClearColor(0, 0, 0, 1.0);

	// Clear the window and the depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//set the camera as default
	//if(ThirdCameraMode)camera->enable();

	Shader* current_shader = world.current_shader;

	if(current_shader)
	{

	
		//set flags
		glDisable(GL_BLEND);

		glDisable(GL_DEPTH_TEST);
		
		glDisable(GL_CULL_FACE);

		//glViewport(0, 0, window_width / 2.0f, window_height);
		world.renderSkybox();

		//Double ViewPort
		/*glViewport(window_width / 2.0f, 0, window_width / 2.0f, window_height);
		world2.renderSkybox();*/

		glEnable(GL_DEPTH_TEST);

		world.renderentities();

		world.renderplane();
		
		world.renderGUI();

		//world.water.render();
		

		
	}

	//Draw the floor grid
	//drawGrid();

	//render the FPS, Draw Calls, etc
	drawText(2, 2, getGPUStats(), Vector3(1, 1, 1), 2);

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
	if (Input::isKeyPressed(SDL_SCANCODE_LSHIFT) ) speed *= 10; //move faster with left shift

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
		world.Player->update(seconds_elapsed, world.props);
		if (Input::isKeyPressed(SDL_SCANCODE_F)) {
			world.Player->grab(world.bullets_and_cannon);
		}
		if (Input::isKeyPressed(SDL_SCANCODE_G)) {
			world.Player->throwItem();
		}
	}

	world.Player->updateAnim(time);
	world.Titan->update(seconds_elapsed*speed, world.props);
	world.update(elapsed_time);
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
	switch (event.keysym.sym)
	{
	case SDLK_r:
		if (!ThirdCameraMode && world.Player->isoncannon) {
			world.Player->shoot(elapsed_time*speed);
		}
		break;
	case SDLK_SPACE:
		if (instructions > -1) {
			world.GUIs[instructions].enable = false;
			instructions--;
		}

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

