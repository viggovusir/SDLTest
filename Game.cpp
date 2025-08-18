#include "Game.h"
#include "Input.h"
#include <iostream>
#include "ECS/Systems/RenderSystem.cpp"
#include "ECS/Types.h"
#include "ECS/Systems/PlayerSystem.cpp"
#include "ECS/Systems/PhysicsSystem.cpp"
#include "ECS/Systems/BallSystem.cpp"
#include "Circle.h"

SDL_Event Game::event;
Input input;
double Game::DeltaTime = 0;

int Game::sceneWidth = 480;
int Game::sceneHeight = 270;

SDL_FRect drawRect = { 0,0,0,0 };

EntityID player;

Game::Game() : camera()
{
	title = "Redacted Engine";
	scaleFactor = 3;
	windowWidth = sceneWidth * scaleFactor;
	windowHeight = sceneHeight * scaleFactor;
	isRunning = true;
	window = nullptr;
	renderer = nullptr;
	renderTarget = nullptr;
	viewportRect = { 0,0,0,0 };
	x = 0;
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
	auto renderSystem = entityManager.RegisterSystem<RenderSystem>();
	entityManager.AddComponentToSystem<RenderSystem, Transform>();
	entityManager.AddComponentToSystem<RenderSystem, Sprite>();

	auto playerSystem = entityManager.RegisterSystem<PlayerSystem>();
	entityManager.AddComponentToSystem<PlayerSystem, Player>();
	//entityManager.AddComponentToSystem<PlayerSystem, Transform>();
	entityManager.AddComponentToSystem<PlayerSystem, RigidBody>();

	auto physicsSystem = entityManager.RegisterSystem<PhysicsSystem>();
	entityManager.AddComponentToSystem<PhysicsSystem, Transform>();
	entityManager.AddComponentToSystem<PhysicsSystem, RigidBody>();

	auto ballSystem = entityManager.RegisterSystem<BallSystem>();
	entityManager.AddComponentToSystem<BallSystem, Ball>();
	entityManager.AddComponentToSystem<BallSystem, RigidBody>();
	entityManager.AddComponentToSystem<BallSystem, Transform>();

	player = entityManager.CreateEntity();
	entityManager.AddComponentToEntity(player, Transform{ Vector2(sceneWidth / 2,sceneHeight / 2), Vector2(8, 15), Vector2(3.5,14) });
	entityManager.AddComponentToEntity(player, Sprite{ 255,255,255,255 });
	entityManager.AddComponentToEntity(player, Player{ 125 });
	entityManager.AddComponentToEntity(player, RigidBody{ Vector2(0,0), 2 });

	auto e2 = entityManager.CreateEntity();
	entityManager.AddComponentToEntity(e2, Transform{ Vector2(20,20) });
	entityManager.AddComponentToEntity(e2, Sprite{ 0,255,255,255 });
	entityManager.AddComponentToEntity(e2, Ball{ 1 });
	entityManager.AddComponentToEntity(e2, RigidBody{ Vector2(0,0) });

	for (int i = 0; i < 90; i++) {
		auto _e = entityManager.CreateEntity();
		entityManager.AddComponentToEntity(_e, Transform{ Vector2(std::rand() % (sceneWidth*3), std::rand() % (sceneHeight*3)) , Vector2(5, 30), Vector2(2,29) });
		entityManager.AddComponentToEntity(_e, Sprite{ 79,52,47, 255 });
	}

	for (int i = 0; i < 90; i++) {
		auto _e = entityManager.CreateEntity();
		entityManager.AddComponentToEntity(_e, Transform{ Vector2(std::rand() % (sceneWidth*3), std::rand() % (sceneHeight*3)) , Vector2(7,5), Vector2(3,4) });
		entityManager.AddComponentToEntity(_e, Sprite{ 166, 158, 154, 255 });
	}

	/*entityManager.AddComponentToSystem<RenderSystem, Position>();

	for (int x = 0; x < sceneWidth; x++) {
		for (int y = 0; y < sceneHeight; y++) {
			EntityID e = entityManager.CreateEntity();
			entityManager.AddComponentToEntity(e, Transform{ Vector2(x,y) });
			entityManager.AddComponentToEntity(e, Color{ (x+y)>>2,x >> 2,y >> 2,255});
			entityManager.AddComponentToEntity(e, Position{ 2,2});
		}
	}*/
	//renderSystem->PrintComponents();

	SDL_Init(SDL_INIT_VIDEO);

	window = SDL_CreateWindow(title, windowWidth, windowHeight, SDL_WINDOW_OPENGL);

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

		SDL_Delay(1);
	}

	SDL_Quit();
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
	for (std::pair<const char*, std::shared_ptr<System>> pair : entityManager.GetSystems()) {
		pair.second->Update();
		Transform& t = entityManager.GetComponent<Transform>(player);
		camera.target = &t;
		camera.Update();
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