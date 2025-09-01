#pragma once
#include <SDL3/SDL.h>
#include "Camera.h"

class Scene {
public:
	virtual void Init(SDL_Renderer*& renderer) const = 0;
	virtual void Update(Camera& camera) const = 0;
	virtual void Draw(SDL_Renderer*& renderer, Camera& camera) const = 0;
};