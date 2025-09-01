#include "Game.h"
#include "Input.h"
#include <iostream>
#include "ECS/Systems/RenderSystem.cpp"
#include "ECS/Types.h"
#include "ECS/Systems/PlayerSystem.cpp"
#include "ECS/Systems/PhysicsSystem.cpp"
#include "../Circle.h"
#include "Managers/AssetManager.h"
#include "../Rope.h"

SDL_Event Game::event;
Input input;
double Game::DeltaTime = 0;

int Game::sceneWidth = 470;
int Game::sceneHeight = 280;

SDL_FRect drawRect = { 0,0,0,0 };

EntityID Game::player = 0;

bool Game::inDebugMode = false;

Rope rope = {};

Game::Game() : camera()
{
	title = "Beesiness Endeavor";
	scaleFactor = 3;
	windowWidth = sceneWidth * scaleFactor;
	windowHeight = sceneHeight * scaleFactor;
	isRunning = true;
	window = nullptr;
	renderer = nullptr;
	renderTarget = nullptr;
	viewportRect = { 0,0,0,0 };
}

Game::~Game()
{
}

void Game::Run()
{
	Init();
	input = Input();
}

void Game::Init()
{
	SDL_SetRenderVSync(renderer, 1);

	SDL_Init(SDL_INIT_VIDEO);

	window = SDL_CreateWindow(title, windowWidth, windowHeight, SDL_WINDOW_RESIZABLE);

	if (window == NULL) {
		std::cout << "Could not create window\n";
	}
	else {
		std::cout << "Window created successfully\n";
	}

	renderer = SDL_CreateRenderer(window, NULL);
	if (renderer == NULL) {
		std::cout << "Could not create renderer\n";
	}
	else {
		SDL_SetRenderDrawColor(renderer, 172, 82, 102, 255);
		std::cout << "Renderer created successfully\n";
	}


	renderTarget = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, sceneWidth, sceneHeight);
	SDL_SetTextureScaleMode(renderTarget, SDL_SCALEMODE_NEAREST);

	viewportRect = { 0,0,(float)windowWidth,(float)windowHeight };

	auto renderSystem = entityManager.RegisterSystem<RenderSystem>();
	entityManager.AddComponentToSystem<RenderSystem, Transform>();
	entityManager.AddComponentToSystem<RenderSystem, Sprite>();

	auto playerSystem = entityManager.RegisterSystem<PlayerSystem>();
	entityManager.AddComponentToSystem<PlayerSystem, Player>();
	entityManager.AddComponentToSystem<PlayerSystem, Transform>();
	entityManager.AddComponentToSystem<PlayerSystem, RigidBody>();

	auto physicsSystem = entityManager.RegisterSystem<PhysicsSystem>();
	entityManager.AddComponentToSystem<PhysicsSystem, Transform>();
	//entityManager.AddComponentToSystem<PhysicsSystem, RigidBody>();
	entityManager.AddComponentToSystem<PhysicsSystem, AABBBoundingBox>();

	player = entityManager.CreateEntity();
	entityManager.AddComponentToEntity(player, Transform{ Vector2(sceneWidth / 2,sceneHeight / 2), Vector2(8, 8), Vector2(1,6) });
	entityManager.AddComponentToEntity(player, Sprite{
		AssetManager::LoadTexture("assets/been.png", renderer),
		0, 0, 8, 8,
		255, 255, 255, 255
		});
	entityManager.AddComponentToEntity(player, Player{ 65 });
	entityManager.AddComponentToEntity(player, RigidBody{ Vector2(0,0), 2 });
	entityManager.AddComponentToEntity(player, AABBBoundingBox{ Vector2(0,0), Vector2(5,2), Vector2(3,17)});

		for (int i = 0; i < 90; i++) {
			auto _e = entityManager.CreateEntity();
			entityManager.AddComponentToEntity(_e, Transform{ Vector2(std::rand() % (sceneWidth * 3), std::rand() % (sceneHeight * 3)) , Vector2(13, 49), Vector2(6,46) });
			entityManager.AddComponentToEntity(_e, Sprite{ AssetManager::LoadTexture("assets/tree.png", renderer), 0,0,13, 49, 255, 255, 255, 255 });
			entityManager.AddComponentToEntity(_e, AABBBoundingBox{ Vector2(0,0), Vector2(13,4), Vector2(0,44) });
			//entityManager.AddComponentToEntity(_e, RigidBody{ Vector2(0,0), 2 });
		}

	Uint64 currentTick = SDL_GetTicks();
	Uint64 previousTick = 0;
	while (isRunning) {
		SDL_PollEvent(&event);
		previousTick = currentTick;
		currentTick = SDL_GetTicks();
		DeltaTime = (currentTick - previousTick) / 1000.0f;
		HandleEvents();
		Update();
		Draw();

		if (event.type == SDL_EVENT_WINDOW_RESIZED) {
			ResizeWindow(event.window.data1, event.window.data2);
		}

		SDL_Delay(1);
	}

	SDL_Quit();
}

void Game::ResizeWindow(int windowWidth, int windowHeight) {
	sceneWidth = (windowWidth % scaleFactor);
	sceneHeight = (windowHeight % scaleFactor);
}

void Game::HandleEvents() {


	/*switch (event.type) {
	case SDL_EVENT_QUIT:
		isRunning = false;
		break;
	}*/

	if (event.type == SDL_EVENT_KEY_DOWN) {
		if (event.key.key == SDLK_ESCAPE)
			isRunning = false;
	}

	input.handleInput();
}

void Game::Update() {
	rope.Update();

	for (std::pair<const char*, std::shared_ptr<System>> pair : entityManager.GetSystems()) {
		pair.second->Update(camera);
		Transform& t = entityManager.GetComponent<Transform>(player);
		camera.target = &t;
		camera.Update();
	}


	if (Input::keyDown(SDL_SCANCODE_E)) {
		int e = entityManager.CreateEntity();
		entityManager.AddComponentToEntity(e, Transform{ Vector2(std::rand() % sceneWidth,std::rand() % sceneHeight), Vector2(11, 19), Vector2(5,17)});
		entityManager.AddComponentToEntity(e, Sprite{
			AssetManager::LoadTexture("assets/Otto.png", renderer),
			0, 0, 11, 19,
			255, 255, 255, 255
			});
		entityManager.AddComponentToEntity(e, Player{ 65 });
		entityManager.AddComponentToEntity(e, RigidBody{ Vector2(0,0), 2 });
		entityManager.AddComponentToEntity(e, AABBBoundingBox{ Vector2(0,0), Vector2(5,2), Vector2(3,17) });
	}
}

void Game::StartDrawing() {
	SDL_SetRenderTarget(renderer, renderTarget);
	SDL_RenderClear(renderer);

}

void Game::Draw() {

	SDL_SetRenderTarget(renderer, renderTarget);
	SDL_RenderClear(renderer);
	// Draw here
	/*for (int x = 0; x < sceneWidth; x++) {
		for (int y = 0; y < sceneHeight; y++) {
			SDL_SetRenderDrawColor(renderer, y >> this->x, x >> this->x, y >> this->x, 255);
			SDL_RenderPoint(renderer, x, y);

		}
	}*/


	for (std::pair<const char*, std::shared_ptr<System>> pair : entityManager.GetSystems()) {
		pair.second->Draw(renderer, camera);
	}

	rope.Draw(renderer);

	SDL_SetRenderDrawColor(renderer, 248, 197, 58, 255);
	//Circle::DrawCircle(renderer, 50, 50, 11);

	SDL_SetRenderDrawColor(renderer, 97, 104, 58, 255);
	SDL_SetRenderTarget(renderer, NULL);

	SDL_RenderTexture(renderer, renderTarget, NULL, &viewportRect);

	SDL_RenderPresent(renderer);
}



void Game::EndDrawing() {
	SDL_SetRenderDrawColor(renderer, 172, 82, 102, 255);
	SDL_SetRenderTarget(renderer, NULL);

	SDL_RenderTexture(renderer, renderTarget, NULL, &viewportRect);

	SDL_RenderPresent(renderer);
}