#pragma once
#include <SDL3/SDL.h>
#include "ECS/EntityManager.h"
#include "Camera.h"
#include "Scene.h"


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

	static bool inDebugMode;

	static SDL_Event event;

	static double DeltaTime;

	static int sceneWidth;
	static int sceneHeight;
	
	static EntityID player;

private:
	void ResizeWindow(int width, int height);

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