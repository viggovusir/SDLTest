#pragma once
#include "Math/Vector2.h"
#include "ECS/Components.h"
#include <SDL3/SDL.h>

class Camera
{
public:
	Camera();
	Camera(float _smoothingFactor, Transform* _target);
	~Camera();


	float smoothingFactor;
	Transform* target = nullptr;
	Vector2 position;

	SDL_FRect viewportRect;

	void Update();
};

