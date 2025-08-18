#pragma once
#include <SDL3/SDL.h>
#include "ECS/EntityManager.h"
#include "Camera.h"


class Game {
public:
	Game();
	~Game();

	void Run();
	void Init();
	void HandleEvents();
	void Update();
	void StartDrawing();
	void Draw();
	void EndDrawing();
	bool isRunning;

	int x;

	static SDL_Event event;

	static double DeltaTime;

	static int sceneWidth;
	static int sceneHeight;
	
private:
	const char* title;
	int scaleFactor;
	int windowWidth;
	int windowHeight;
	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Texture* renderTarget;
	SDL_FRect viewportRect;
	EntityManager entityManager;
	Camera camera;
};