#pragma once
#include <vector>
#include <memory>
#include "../Archetype.h"
#include "../Types.h"
#include <SDL3/SDL.h>
#include "../../Camera.h"
//#include "EntityManager.h"

class EntityManager;

class System {
public:
	friend class EntityManager;
	std::vector<std::shared_ptr<Archetype>> archetypes;

	void AttachEntityManager(EntityManager& _entityManager) {
		m_entityManager = &_entityManager;
	}

	virtual void Update(Camera& camera) const = 0;
	virtual void Draw(SDL_Renderer*& renderer, Camera& camera) const = 0;

protected:
	EntityManager& entityManager() const {
		return *m_entityManager;
	}

private:
	ArchetypeSignature archetypeSignature;

	EntityManager* m_entityManager = nullptr;
};