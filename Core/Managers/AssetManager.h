#pragma once
#include <SDL3/SDL.h>
#include <SDL3/SDL_image.h>

class AssetManager {
public:
	static SDL_Texture* LoadTexture(const char* fileName, SDL_Renderer* renderer);
};