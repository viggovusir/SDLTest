#include "Archetype.h"

Archetype::Archetype(ArchetypeSignature signature) : signature{ signature }, numEntities{ 0 } {

}

size_t Archetype::GetNumEntities() const {
	return numEntities;
}

void Archetype::RemoveComponent(const EntityID& entity, const char*& componentTypeName) {
	components.at(componentTypeName)->RemoveComponent(entity);
}

void Archetype::EntityDestroyed(const EntityID& entity) {
	for (std::pair<const char*, std::shared_ptr<IComponentCollection>> pair : components) {
		pair.second->EntityDestroyed(entity);
	}
	numEntities--;
}

std::shared_ptr<IComponentCollection> Archetype::GetComponents(const char*& componentTypeName) {
	return components.at(componentTypeName);
}

void Archetype::TransferComponents(const EntityID& entity, std::shared_ptr<Archetype> newArchetype) {
	for (std::pair<const char*, std::shared_ptr<IComponentCollection>> pair : components) {
		if (newArchetype->components.count(pair.first) == 0) {
			newArchetype->components.insert({ pair.first, pair.second->CreateEmptyClone() });
		}

		pair.second->TransferComponents(entity, newArchetype->GetComponents(pair.first));

		RemoveComponent(entity, pair.first);
	}
}