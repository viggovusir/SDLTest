#include <SDL3/SDL.h>
#include "../System.h"
#include "../Components.h"
#include <iostream>
#include "../../Game.h"

class PhysicsSystem : public System {

	void Update() const override {

		for (std::shared_ptr<Archetype> archetype : archetypes) {
			std::vector<Transform>& transforms = archetype->GetComponents<Transform>();
			std::vector<RigidBody>& rigidBodies = archetype->GetComponents<RigidBody>();

			for (size_t i = 0; i < archetype->GetNumEntities(); ++i) {
				Transform& t = transforms[i];


				t.position.x += rigidBodies[i].velocity.x * Game::DeltaTime;
				t.position.y += rigidBodies[i].velocity.y * Game::DeltaTime;

				/*if (t.position.x <= 0 + t.localOrigin.x)
					t.position.x = 0 + t.localOrigin.x;
				if (t.position.x >= Game::sceneWidth - t.scale.x + t.localOrigin.x)
					t.position.x = Game::sceneWidth - t.scale.x + t.localOrigin.x;
				if (t.position.y <= 0 + t.localOrigin.y)
					t.position.y = 0 + t.localOrigin.y;
				if (t.position.y >= Game::sceneHeight - t.scale.y + t.localOrigin.y)
					t.position.y = Game::sceneHeight - t.scale.y + t.localOrigin.y;*/
			}
		}
	}

	void Draw(SDL_Renderer*& renderer, Camera& camera) const override {}
};