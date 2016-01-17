#pragma once
#include <string>
#include <SDL.h>
#include <SDL_image.h>

//Texture wrapper class
class LTexture
{
public:
	//Initializes variables
	LTexture();

	//Deallocates memory
	~LTexture();

	//Loads image at specific path
	bool loadFromFile(std::string Path, SDL_Renderer*& gRenderer);

	//Deallocates textures
	void free();

	//Render texture at given point
	void render(int x, int y, SDL_Renderer*& gRenderer, SDL_Rect* clip = NULL);

	//Gets image dimensions
	int getWidth();
	int getHeight();

private:
	//Actual hardware texture
	SDL_Texture* mTexture;

	//Image dimensions
	int mWidth;
	int mHeight;
};

