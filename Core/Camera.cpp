#include "Camera.h"
#include "Game.h"

Camera::Camera() {
	viewportRect = { 0,0,0,0 };
	smoothingFactor = 0.9f;
}

Camera::Camera(float _smoothingFactor, Transform* _target) {
	viewportRect = { 0,0,0,0 };
}

Camera::~Camera() {

}

void Camera::Update() {
	position.x += ((target->position.x - Game::sceneWidth / 2) - position.x) / smoothingFactor * Game::DeltaTime;
	position.y += ((target->position.y - Game::sceneHeight / 2) - position.y) / smoothingFactor * Game::DeltaTime;

	if (position.x < 0) position.x = 0;
	if (position.y < 0)	position.y = 0;

	viewportRect.x = position.x + Game::sceneWidth / 2;
	viewportRect.y = position.y + Game::sceneHeight / 2;
	viewportRect.w = Game::sceneWidth;
	viewportRect.h = Game::sceneHeight;
}
