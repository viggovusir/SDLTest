#pragma once
#include <vector>
#include <memory>
#include "Archetype.h"
#include "Types.h"
#include <SDL3/SDL.h>
#include "../Camera.h"


class System {
public:
	friend class EntityManager;
	std::vector<std::shared_ptr<Archetype>> archetypes;

	virtual void Update() const = 0;
	virtual void Draw(SDL_Renderer*& renderer, Camera& camera) const = 0;

private:
	ArchetypeSignature archetypeSignature;
};