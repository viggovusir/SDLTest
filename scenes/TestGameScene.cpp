#include "../Core/Scene.h"
#include "../Core/ECS/Types.h"
#include "../Core/ECS/EntityManager.h"
#include "../Core/ECS/Systems/RenderSystem.cpp"
#include "../Core/ECS/Systems/PlayerSystem.cpp"
#include "../Core/ECS/Systems/PhysicsSystem.cpp"
#include "../Core/Managers/AssetManager.h"



class TestGameScene : public Scene {
public:
	EntityID& player ;
	EntityManager& entityManager;
	void Init(SDL_Renderer*& renderer) const override {
		auto renderSystem = entityManager.RegisterSystem<RenderSystem>();
		entityManager.AddComponentToSystem<RenderSystem, Transform>();
		entityManager.AddComponentToSystem<RenderSystem, Sprite>();

		auto playerSystem = entityManager.RegisterSystem<PlayerSystem>();
		entityManager.AddComponentToSystem<PlayerSystem, Player>();
		entityManager.AddComponentToSystem<PlayerSystem, Transform>();
		entityManager.AddComponentToSystem<PlayerSystem, RigidBody>();

		auto physicsSystem = entityManager.RegisterSystem<PhysicsSystem>();
		entityManager.AddComponentToSystem<PhysicsSystem, Transform>();
		entityManager.AddComponentToSystem<PhysicsSystem, RigidBody>();

		player = entityManager.CreateEntity();
		entityManager.AddComponentToEntity(player, Transform{ Vector2(Game::sceneWidth / 2,Game::sceneHeight / 2), Vector2(11, 19), Vector2(5,17) });
		entityManager.AddComponentToEntity(player, Sprite{
			AssetManager::LoadTexture("assets/Otto.png", renderer),
			0, 0, 11, 19,
			255, 255, 255, 255
			});
		entityManager.AddComponentToEntity(player, Player{ 65 });
		entityManager.AddComponentToEntity(player, RigidBody{ Vector2(0,0), 2 });

		for (int i = 0; i < 90; i++) {
			auto _e = entityManager.CreateEntity();
			entityManager.AddComponentToEntity(_e, Transform{ Vector2(std::rand() % (Game::sceneWidth * 3), std::rand() % (Game::sceneHeight * 3)) , Vector2(13, 49), Vector2(6,48) });
			entityManager.AddComponentToEntity(_e, Sprite{ AssetManager::LoadTexture("assets/monolith.png", renderer), 0,0,13, 49, 255, 255, 255, 255 });
		}
	}

	void Update(Camera& camera) const override {
		for (std::pair<const char*, std::shared_ptr<System>> pair : entityManager.GetSystems()) {
			pair.second->Update(camera);
			Transform& t = entityManager.GetComponent<Transform>(player);
			camera.target = &t;
			camera.Update();
		}
	}

	void Draw(SDL_Renderer*& renderer, Camera& camera) const override {
		for (std::pair<const char*, std::shared_ptr<System>> pair : entityManager.GetSystems()) {
			pair.second->Draw(renderer, camera);
		}
	}

private:

};