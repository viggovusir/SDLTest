#include "AssetManager.h"
#include <cassert>

SDL_Texture* AssetManager::LoadTexture(const char* fileName, SDL_Renderer* renderer) {
	SDL_Surface* tempSurface = IMG_Load(fileName);
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, tempSurface);
	assert(tempSurface != NULL && "Couldn't load texture");

	//SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_NONE);

	SDL_DestroySurface(tempSurface);

	return texture;
}