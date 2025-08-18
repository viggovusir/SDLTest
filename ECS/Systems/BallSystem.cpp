#include "../System.h"
#include "../Components.h"
#include "../../Game.h"
#include "../../Input.h"

class BallSystem : public System {
public:
	void Update() const override {
		for (std::shared_ptr<Archetype> archetype : archetypes) {
			std::vector<Ball>& balls = archetype->GetComponents<Ball>();
			std::vector<RigidBody> rbs = archetype->GetComponents<RigidBody>();
			std::vector<Transform> transforms = archetype->GetComponents<Transform>();

			for (int i = 0; i < archetype->GetNumEntities(); ++i) {
				//transforms[i].position.x += 500 * Game::DeltaTime;
				if (Input::keyDown(SDL_SCANCODE_E))
					rbs[i].velocity.x = 5;

				if (transforms[i].position.x <= 0)
					rbs[i].velocity.x = -rbs[i].velocity.x;
				if (transforms[i].position.x >= Game::sceneWidth)
					rbs[i].velocity.x = -rbs[i].velocity.x;
				if (transforms[i].position.y <= 0)
					rbs[i].velocity.y = -rbs[i].velocity.y;
				if (transforms[i].position.x >= Game::sceneHeight)
					rbs[i].velocity.y = -rbs[i].velocity.y;
			}
		}
	}

	void Draw(SDL_Renderer*& renderer, Camera& camera) const override{}
};