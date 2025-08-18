#pragma once
#include "Types.h"
#include "ComponentCollection.h"
#include <unordered_map>
#include <memory>

class Archetype {
public:
	Archetype(ArchetypeSignature signature);

	friend class EntityManager;

	size_t GetNumEntities() const;

	template<typename T>
	std::vector<T>& GetComponents() {
		const char* componentTypeName = typeid(T).name();
		return std::static_pointer_cast<ComponentCollection<T>>(components.at(componentTypeName))->GetComponents();
	}

private:
	ArchetypeSignature signature;
	size_t numEntities;
	std::unordered_map<const char*, std::shared_ptr<IComponentCollection>> components;

	void RemoveComponent(const EntityID& entity, const char*& componentTypeName);
	void EntityDestroyed(const EntityID& entity);
	std::shared_ptr<IComponentCollection> GetComponents(const char*& componentTypeName);
	void TransferComponents(const EntityID& entity, std::shared_ptr<Archetype> newArchetype);

	template<typename T>
	void AddComponent(EntityID entity, T component) {
		const char* componentTypeName = typeid(T).name();

		if (components.find(componentTypeName) == components.end()) {
			components.insert({ componentTypeName, std::make_shared<ComponentCollection<T>>() });
		}

		std::static_pointer_cast<ComponentCollection<T>>(components.at(componentTypeName))->AddComponent(entity, component);
	}
};