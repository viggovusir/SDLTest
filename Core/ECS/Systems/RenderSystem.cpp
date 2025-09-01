#include "System.h"
#include <iostream>
#include "../Components.h"
#include <SDL3/SDL_render.h>
#include "../../Input.h"
#include <algorithm>
#include "../../Game.h"
#include <cmath>
#include "../../Math/Math.h"

struct RenderItem {
	Transform* transform;
	Sprite* sprite;
	int index;

};

class RenderSystem : public System {
public:

	void PrintComponents() {
		std::cout << "Render System: \n \n";

		for (std::shared_ptr<Archetype> archetype : archetypes) {
			std::vector<Transform>& transforms = archetype->GetComponents<Transform>();
			std::vector<Sprite>& sprites = archetype->GetComponents<Sprite>();

			for (size_t i = 0; i < archetype->GetNumEntities(); ++i) {
				std::cout << "Position: " << transforms[i].position.x << "," << transforms[i].position.y << "\n";
				std::cout << "Color: " << sprites[i].r << "," << sprites[i].g << "," << sprites[i].b << "," << sprites[i].a << "\n";
			}
		}
	}

	void Update(Camera& camera) const override {
		SetRenderOrder();

		auto& playerTransform = entityManager().GetComponent<Transform>(Game::player);
		auto& playerSprite = entityManager().GetComponent<Sprite>(Game::player);

		//Transform& t = entityManager->GetComponent<Transform>(Game::player);

		for (const RenderItem& item : renderQueue) {
			item.transform->rect = { std::roundf(item.transform->position.x - item.transform->localOrigin.x - camera.position.x), std::roundf(item.transform->position.y - item.transform->localOrigin.y - camera.position.y), item.transform->scale.x, item.transform->scale.y };

			if (SDL_HasRectIntersectionFloat(&item.transform->rect, &playerTransform.rect)
				&& item.transform->position.y > playerTransform.position.y) {
				item.sprite->a = RedactedMath::LerpF(item.sprite->a, 125, 0.2f, Game::DeltaTime);
			}
			else
				item.sprite->a = RedactedMath::LerpF(125, 255, 0.7f, Game::DeltaTime);
		}
	}

	void Draw(SDL_Renderer*& renderer, Camera& camera) const override {
		for (const RenderItem& item : renderQueue) {
			//SDL_SetTextureBlendMode(item.sprite->texture, SDL_BLENDMODE_BLEND);
			//SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);



			SDL_SetRenderDrawColor(renderer, item.sprite->r, item.sprite->g, item.sprite->b, 255);
			SDL_SetTextureAlphaMod(item.sprite->texture, item.sprite->a);
			SDL_RenderTexture(renderer, item.sprite->texture, item.sprite->GetRect(), item.transform->GetRect());

			// DEBUG DRAW
			if (Game::inDebugMode) {
				SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);
				SDL_RenderPoint(renderer, item.transform->position.x - camera.position.x, item.transform->position.y - camera.position.y);
			}
		}

		for (std::shared_ptr<Archetype> archetype : archetypes) {

		}
	}

private:
	mutable std::vector<RenderItem> renderQueue;

	void SetRenderOrder() const {
		renderQueue.clear();

		size_t totalEntities = 0;
		for (auto& archetype : archetypes)
			totalEntities += archetype->GetNumEntities();

		renderQueue.reserve(totalEntities);

		for (std::shared_ptr<Archetype> archetype : archetypes) {
			auto& transforms = archetype->GetComponents<Transform>();
			auto& sprites = archetype->GetComponents<Sprite>();

			for (size_t i = 0; i < archetype->GetNumEntities(); ++i) {
				renderQueue.push_back(RenderItem{ &transforms[i], &sprites[i], (int)i });
				sprites[i].layerDepth = i / 100;
			}
		}
		std::stable_sort(renderQueue.begin(), renderQueue.end(), [](const RenderItem& a, const RenderItem& b) {
			return (a.transform->position.y) < (b.transform->position.y);
			});

	}
};

