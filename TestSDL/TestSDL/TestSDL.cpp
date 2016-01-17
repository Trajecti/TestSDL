#include "stdafx.h"
#include <SDL.h>
#include <SDL_image.h>
#undef main
#include <iostream>
#include <string>

//screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

//Key press surfaces constants
enum KeyPressSurfaces
{
	KEY_PRESS_SURFACE_DEFAULT,
	KEY_PRESS_SURFACE_UP,
	KEY_PRESS_SURFACE_DOWN,
	KEY_PRESS_SURFACE_LEFT,
	KEY_PRESS_SURFACE_RIGHT,
	KEY_PRESS_SURFACE_TOTAL
};

//Initalizes SDL and creates window
bool init();

//Loads media
bool loadMedia();

//Frees media and shuts down SDL
void close();

//The window to be rendered to
SDL_Window* gWindow = NULL;

//loads individual images
SDL_Surface* loadSurface(std::string path);

//Surface contained by the window
SDL_Surface* gScreenSurface = NULL;

//Images associated to a keypress
SDL_Surface* gKeyPressSurfaces[KEY_PRESS_SURFACE_TOTAL];


//Loads individual image as texture
SDL_Texture* loadTexture(std::string path);

//The window renderer
SDL_Renderer* gRenderer = NULL;

//Currently displayed texture
SDL_Texture* gTexture = NULL;

SDL_Surface* loadSurface(std::string path) {

	//The final optimized image
	SDL_Surface* optimizedSurface = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());

	if (loadedSurface == NULL) {
		std::cout << "Unable to load image "<< path << " ! SDL_Error: " << IMG_GetError() << '\n';
	}
	else {
		//Convert surface to screen format
		optimizedSurface = SDL_ConvertSurface(loadedSurface, gScreenSurface->format, NULL);
		if (optimizedSurface == NULL) {
			std::cout << "Unable to optimize image " << path << " ! SDL_Error: " << SDL_GetError() << '\n';
		}

		//get rid of old surface
		SDL_FreeSurface(loadedSurface);
	}
	return optimizedSurface;
}

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
			// Create renderer for window
			gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
			if (gRenderer == NULL) {
				std::cout << "Renderer could not be created! SDL Error: " << SDL_GetError() << '\n';
				success = false;
			}
			else {

				//Initialize renderer color
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if (!(IMG_Init(imgFlags) & imgFlags))
				{
					printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
					success = false;
				}
				else
				{
					//Get window surface
					gScreenSurface = SDL_GetWindowSurface(gWindow);
				}
			}

		}
	}

	return success;
}

SDL_Texture* loadTexture(std::string path) {
	//The final texture
	SDL_Texture* newTexture = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == NULL) {
		std::cout << "Unable to load image! SDL_image Error: " << path << " " << IMG_GetError() << '\n';
	}
	else {
		//Create texture from surface pixels
		newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
		if (newTexture == NULL) {
			std::cout << "Unable to create texture from SDL_image from : " << path << "! Error: " << SDL_GetError() << '\n';
		}
		//Get red of old loaded surface
		SDL_FreeSurface(loadedSurface);
	}
	return newTexture;
}

bool loadMedia()
{
	//Loading success flag
	bool success = true;

	//Load PNG texture
	gTexture = loadTexture("media/texture.png");
	if (gTexture == NULL) {
		std::cout << "Failed to load texture image! \n";
		success = false;
	}
	

	return success;
}

void close() {

	//Free loaded image
	SDL_DestroyTexture(gTexture);
	gTexture = NULL;

	//Destroy window
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
	IMG_Quit();
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

		//Clear screen
		SDL_RenderClear(gRenderer);

		//Render texture to screen
		SDL_RenderCopy(gRenderer, gTexture, NULL, NULL);

		//Update screen
		SDL_RenderPresent(gRenderer);
	}

	//Free resources and close SDL
	close();
    return 0;
}

