#include "../System.h"
#include <iostream>
#include "../Components.h"
#include <SDL3/SDL_render.h>
#include "../../Input.h"
#include <algorithm>

struct RenderItem {
	Transform* transform;
	Sprite* sprite;
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

	void Update() const override {
		SetRenderOrder();
	}

	void Draw(SDL_Renderer*& renderer, Camera& camera) const override {
		for (const RenderItem& item : renderOrder) {
			SDL_FRect rect = { item.transform->position.x - item.transform->localOrigin.x - camera.position.x, item.transform->position.y - item.transform->localOrigin.y - camera.position.y, item.transform->scale.x, item.transform->scale.y };

			SDL_SetRenderDrawColor(renderer, item.sprite->r, item.sprite->g, item.sprite->b, item.sprite->a);
			SDL_RenderFillRect(renderer, &rect);

			SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);
			SDL_RenderPoint(renderer, item.transform->position.x - camera.position.x, item.transform->position.y - camera.position.y);
		}
	}

private:
	mutable std::vector<RenderItem> renderOrder;

	void SetRenderOrder() const {
		renderOrder.clear();

		size_t totalEntities = 0;
		for (auto& archetype : archetypes)
			totalEntities += archetype->GetNumEntities();

		renderOrder.reserve(totalEntities);

		for (std::shared_ptr<Archetype> archetype : archetypes) {
			auto& transforms = archetype->GetComponents<Transform>();
			auto& sprites = archetype->GetComponents<Sprite>();

			for (size_t i = 0; i < archetype->GetNumEntities(); ++i) {
				renderOrder.push_back(RenderItem{ &transforms[i], &sprites[i] });
			}
		}
		std::stable_sort(renderOrder.begin(), renderOrder.end(), [](const RenderItem& a, const RenderItem& b) {
			return (a.transform->position.y) < (b.transform->position.y);
			});

	}
};

