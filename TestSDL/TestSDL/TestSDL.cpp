#include "stdafx.h"
#include <SDL.h>
#undef main
#include <iostream>

//screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

//Initalizes SDL and creates window
bool init();

//Loads media
bool loadMedia();

//Frees media and shuts down SDL
void close();

//The window to be rendered to
SDL_Window* gWindow = NULL;

//Surface contained by the window
SDL_Surface* gscreenSurface = NULL;

//Image that will load and be displayed on screen
SDL_Surface* gHelloWorld = NULL;

bool init() {

	//Initialization flag
	bool success = true;

	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		std::cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << '\n';
		success = false;
	}
	else {
		//Create window
		gWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (gWindow == NULL) {
			std::cout << "Error: Window could not be created! SDL_Error: " << SDL_GetError() << '\n';
			success = false;
		}
		else {
			//Get Window surface
			gscreenSurface = SDL_GetWindowSurface(gWindow);
		}
	}

	return success;
}

bool loadMedia()
{
	//Loading success flag
	bool success = true;

	//Load splash image
	gHelloWorld = SDL_LoadBMP("media/hello_world.bmp");

	if (gHelloWorld == NULL) {
		std::cout << "Error: Unable to load image  hello_world.bmp ! SDL_Error: " << SDL_GetError() << '\n';
		success = false;
	}
	return success;
}

void close() {

	//Deallocate surface
	SDL_FreeSurface(gHelloWorld);
	gHelloWorld = NULL;

	//Destroy window
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;

	//Quit SDL subsystem
	SDL_Quit();

}

int main()
{
	bool success_init = true;
	// Start up SDL and make window
	if (!init()) {
		std::cout << "Failed to initialize \n";
		success_init = false;
	}
	else {
		if (!loadMedia()) {
			std::cout << "Failed to load media \n";
			success_init = false;
		}
	}

	if (!success_init) { return 0; }

	//Main loop flag
	bool quit = false;

	//Event handler
	SDL_Event e;

	//While app is running
	while (!quit) {
		//Handle events on queue
		while (SDL_PollEvent(&e) != 0) {

			//User requests quit
			if (e.type == SDL_QUIT) {
				quit = true;
			}
		}

		//Apply the image
		SDL_BlitSurface(gHelloWorld, NULL, gscreenSurface, NULL);

		//Update surface
		SDL_UpdateWindowSurface(gWindow);
	}

	//Free resources and close SDL
	close();
    return 0;
}

