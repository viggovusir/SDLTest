#pragma once
#include <SDL3/SDL.h>
#include <map>

class Input {
public:
	Input();
	~Input();

	void handleInput();
	static bool keyPressed(SDL_Scancode key);
	static bool keyDown(SDL_Scancode key);
	static bool keyReleased(SDL_Scancode key);

private:
	static const bool* keyState;
};