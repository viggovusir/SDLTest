#include <SDL3/SDL.h>
#include "../System.h"
#include "../Components.h"
#include "../../Input.h"
#include <iostream>

class PlayerSystem : public System {
public:
	void Update() const override {
		for (std::shared_ptr<Archetype> archetype : archetypes) {
			std::vector<Player>& player = archetype->GetComponents<Player>();
			//std::vector<Transform>& transforms = archetype->GetComponents<Transform>();
			std::vector<RigidBody>& rigidbody = archetype->GetComponents<RigidBody>();

			for (size_t i = 0; i < archetype->GetNumEntities(); ++i) {
					//transforms[i].position.x += 1;
				if (Input::keyDown(SDL_SCANCODE_D)) {
					//transforms[i].position.x++;
					rigidbody[i].velocity.x = player[i].moveSpeed;

					std::cout << rigidbody[i].velocity.x << "\n";
				}
				else if (Input::keyDown(SDL_SCANCODE_A)) {
					rigidbody[i].velocity.x = -player[i].moveSpeed;

				}
				else {
					rigidbody[i].velocity.x = 0;
				}
				if (Input::keyDown(SDL_SCANCODE_W)) {
					//transforms[i].position.x++;
					rigidbody[i].velocity.y = -player[i].moveSpeed;

					std::cout << rigidbody[i].velocity.y << "\n";
				}
				else if (Input::keyDown(SDL_SCANCODE_S)) {
					rigidbody[i].velocity.y = player[i].moveSpeed;

				}
				else {
					rigidbody[i].velocity.y = 0;
				}
					//rigidbody[i].velocity.x++;
				/*else
					rigidbody[i].velocity.x = 0;*/

		/*		else
					rigidbody[i].velocity.x = 0;*/


			}
		}
	}


	void Draw(SDL_Renderer*& renderer, Camera& camera) const override {

	}
};