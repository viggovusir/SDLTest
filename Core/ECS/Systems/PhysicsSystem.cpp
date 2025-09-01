#pragma once
#include <SDL3/SDL.h>
#include "System.h"
#include "../Components.h"
#include <iostream>
#include "../../Game.h"

static enum CollisionSide {
	None = 0,
	Top = 1,
	Bottom = 2,
	Left = 3,
	Right = 4
};

static CollisionSide GetCollisionSide(AABBBoundingBox& movingBoundingBox, Vector2& movingTransform, AABBBoundingBox& staticBoundingBox) {



	if (movingBoundingBox.Left() + (movingTransform.x * Game::DeltaTime) <= staticBoundingBox.Right() &&
		movingBoundingBox.Right() > staticBoundingBox.Right() &&
		movingBoundingBox.Bottom() > staticBoundingBox.Top() &&
		movingBoundingBox.Top() < staticBoundingBox.Bottom())
	{
		return Left;
	}
	else if (
		movingBoundingBox.Right() + (movingTransform.x * Game::DeltaTime) >= staticBoundingBox.Left() &&
		movingBoundingBox.Left() < staticBoundingBox.Left() &&
		movingBoundingBox.Bottom() > staticBoundingBox.Top() &&
		movingBoundingBox.Top() < staticBoundingBox.Bottom())
	{
		return Right;
	}
	else if (
		movingBoundingBox.Top() + (movingTransform.y * Game::DeltaTime) <= staticBoundingBox.Bottom() &&
		movingBoundingBox.Bottom() > staticBoundingBox.Bottom() &&
		movingBoundingBox.Right() > staticBoundingBox.Left() &&
		movingBoundingBox.Left() < staticBoundingBox.Right())
	{
		return Top;
	}
	else if (
		movingBoundingBox.Bottom() + (movingTransform.y * Game::DeltaTime) >= staticBoundingBox.Top() &&
		movingBoundingBox.Top() < staticBoundingBox.Top() &&
		movingBoundingBox.Right() > staticBoundingBox.Left() &&
		movingBoundingBox.Left() < staticBoundingBox.Right())
	{
		return Bottom;
	}

	return None;
}

static Vector2& GetCorrectedPosition(AABBBoundingBox& movingBoundingBox, AABBBoundingBox& staticBoundingBox, CollisionSide collisionSide) {
	Vector2& correctedPosition = movingBoundingBox.position;
	std::cout << "Getting corrected position" << std::endl;
	switch (collisionSide) {
	case Left:
		correctedPosition.x = staticBoundingBox.Right();
		break;
	case Right:
		correctedPosition.x = staticBoundingBox.Left();
		break;
	case Top:
		correctedPosition.y = staticBoundingBox.Bottom();
		break;
	case Bottom:
		correctedPosition.y = staticBoundingBox.Top();
		break;
	}
	return correctedPosition;
}

class PhysicsSystem : public System {

public:
	float fullStopThreshold = 0.5f; // Put on entity instead??

	void Update(Camera& camera) const override {

		for (std::shared_ptr<Archetype> archetype : archetypes) {
			std::vector<Transform>& transforms = archetype->GetComponents<Transform>();
			std::vector<AABBBoundingBox>& aabb = archetype->GetComponents<AABBBoundingBox>();

			auto* rigidBodies = archetype->TryGetComponents<RigidBody>();

			for (size_t i = 0; i < archetype->GetNumEntities(); ++i) {
				Transform& t = transforms[i];

				aabb[i].position = transforms[i].position;

				if (rigidBodies) {
					auto& rb = (*rigidBodies)[i];

					t.position.x += rb.velocity.x * Game::DeltaTime;
					t.position.y += rb.velocity.y * Game::DeltaTime;

					if (std::abs(rb.velocity.x) < fullStopThreshold)
						rb.velocity.x = 0;
					if (std::abs(rb.velocity.y) < fullStopThreshold)
						rb.velocity.y = 0;

					for (size_t j = 1; j < archetype->GetNumEntities(); ++j) {
						transforms[0].position = GetCorrectedPosition(aabb[0], aabb[j], GetCollisionSide(aabb[0], rb.velocity, aabb[j]));
						//if (i != 0) {
						//}
					}
				}

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

	void Draw(SDL_Renderer*& renderer, Camera& camera) const override {
		if (Game::inDebugMode) {
			for (std::shared_ptr<Archetype> archetype : archetypes) {
				std::vector<Transform>& transforms = archetype->GetComponents<Transform>();
				std::vector<AABBBoundingBox>& aabb = archetype->GetComponents<AABBBoundingBox>();

				for (size_t i = 0; i < archetype->GetNumEntities(); ++i) {
					//SDL_FRect recta = { transforms[i].position.x - transforms[i].localOrigin.x - camera.position.x + aabb[i].offset.x,transforms[i].position.y - transforms[i].localOrigin.y - camera.position.y + aabb[i].offset.y, aabb[i].scale.x, aabb[i].scale.y };
					SDL_FRect rect = { aabb[i].position.x - camera.position.x, aabb[i].position.y - camera.position.y, aabb[i].scale.x, aabb[i].scale.y };
					//SDL_SetRenderDrawColor(renderer, 255, 0, 255, 50);
					SDL_RenderRect(renderer, &rect);
				}
			}
		}
	}
};