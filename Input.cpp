#include "Input.h";
#include "Game.h";
#include <iostream>

const bool* Input::keyState;

Input::Input() {
	keyState = SDL_GetKeyboardState(NULL);
}

Input::~Input() {

}

void Input::handleInput() {
	keyState = SDL_GetKeyboardState(NULL);
}

bool Input::keyDown(SDL_Scancode key) {
	return (keyState[key] ? true : false);
}

bool Input::keyPressed(SDL_Scancode key) {
	if (Game::event.type == SDL_EVENT_KEY_DOWN) {
		if (Game::event.key.key == key && !Game::event.key.repeat) {
			std::cout << SDL_GetKeyName(Game::event.key.key) << "-key pressed\n";
			return true;
		}
		else
			return false;
	}
	return false;
}

bool Input::keyReleased(SDL_Scancode key) {
	return false;
}