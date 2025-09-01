#include "EntityManager.h"
#include <cassert>

EntityManager::EntityManager() {

}

EntityID EntityManager::CreateEntity() {
	EntityID e = nextEntity;
	entityToSignature.insert({ e, ArchetypeSignature() });
	++nextEntity;
	return e;
}

void EntityManager::DestroyEntity(const EntityID& entity) {
	if (entityToSignature.at(entity).any()) {
		std::shared_ptr<Archetype> archetype = archetypes.at(entityToSignature.at(entity));
		archetype->EntityDestroyed(entity);
		entityToSignature.erase(entity);
	}
}

void EntityManager::RegisterComponentType(const char*& typeName) {
//	assert(nextComponentType < MaxComponents && "Maximum component type amount reached");

	componentTypes.insert({ typeName, nextComponentType });

	++nextComponentType;
}

ArchetypeSignature EntityManager::UpdateEntitySignature(const EntityID& entity, const ComponentTypeID& componentType, const bool& isSet) {
	ArchetypeSignature entitySignature = entityToSignature.at(entity);

	if (isSet)
		assert(entitySignature.test(componentType) == false && "Component type already added to entity");

	entitySignature.set(componentType, isSet);
	entityToSignature.at(entity) = entitySignature;

	return entitySignature;
}

void EntityManager::CreateNewArchetype(const ArchetypeSignature& newSignature) {
	assert(archetypes.find(newSignature) == archetypes.end() && "Archetype already added");

	std::shared_ptr<Archetype> newArchetype = std::make_shared<Archetype>(newSignature);

	archetypes.insert({ newSignature, newArchetype });

	for (std::pair<const char*, std::shared_ptr<System>> pair : systems) {
		if ((pair.second->archetypeSignature & newSignature) == pair.second->archetypeSignature) {
			pair.second->archetypes.push_back(archetypes.at(newSignature));
		}
	}
}

void EntityManager::UpdateSystemArchetype(const char*& systemTypeName, ArchetypeSignature& systemArchetypeID) {
	std::shared_ptr<System> system = systems.at(systemTypeName);
	system->archetypes.clear();
	system->archetypeSignature = systemArchetypeID;

	for (std::pair<ArchetypeSignature, std::shared_ptr<Archetype>> pair : archetypes) {
		if ((pair.first & systemArchetypeID) == systemArchetypeID) {
			system->archetypes.push_back(pair.second);
		}
	}
}