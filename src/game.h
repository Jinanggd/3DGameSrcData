/*  by Javi Agenjo 2013 UPF  javi.agenjo@gmail.com
	This class encapsulates the game, is in charge of creating the game, getting the user input, process the update and render.
*/

#ifndef GAME_H
#define GAME_H

#include "includes.h"
#include "camera.h"
#include "utils.h"
#include "entity.h"
#include "world.h"
#include "sound.h"


class Game
{
public:

	static Game* instance;
	//window
	SDL_Window* window;
	int window_width;
	int window_height;

	//some globals
	long frame;
    float time;
	float elapsed_time;
	int fps;
	bool must_exit;
	int loaded = 0;
	GUI Start, LoadingBar,EndGame,ClearGame;
	World world;
	bool isOver = false, isReady = false, isLoading = false, isfullyLoaded = false, renderminimap = false;
	//some vars
	Camera* camera; //our global camera
	Camera* camera2D;
	bool mouse_locked; //tells if the mouse is locked (not seen)
	Sound mysound;

	Game( int window_width, int window_height, SDL_Window* window );

	//main functions
	void render( void );
	void update( double dt );

	//events
	void onKeyDown( SDL_KeyboardEvent event );
	void onKeyUp(SDL_KeyboardEvent event);
	void onMouseButtonDown( SDL_MouseButtonEvent event );
	void onMouseButtonUp(SDL_MouseButtonEvent event);
	void onGamepadButtonDown(SDL_JoyButtonEvent event);
	void onGamepadButtonUp(SDL_JoyButtonEvent event);
	void onResize(int width, int height);
	void GameOver(Vector3 position);
	void renderWorld();
	void renderInit();
	void renderLoading();
	void renderResult();



};


#endif 