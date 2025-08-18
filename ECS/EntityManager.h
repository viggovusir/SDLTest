#pragma once
#include "Archetype.h"
#include "Types.h"
#include "System.h"
#include <unordered_map>
#include <memory>

class EntityManager {
public:
	EntityManager();

	EntityID CreateEntity();
	void DestroyEntity(const EntityID& entity);
	std::unordered_map<const char*, std::shared_ptr<System>>& GetSystems() {
		return this->systems;
	}

	template<typename T>
	void AddComponentToEntity(EntityID entity, T component) {
		const char* componentTypeName = typeid(T).name();

		if (componentTypes.find(componentTypeName) == componentTypes.end()) {
			RegisterComponentType(componentTypeName);
		}

		ComponentTypeID componentTypeID = componentTypes.at(componentTypeName);

		AddComponentToEntityArchetype(entity, component, componentTypeID);
	}

	template<typename T>
	T& GetComponent(EntityID entity) {
		const char* componentTypeName = typeid(T).name();

		assert(componentTypes.find(componentTypeName) != componentTypes.end() && "Component type not registered");

		ArchetypeSignature signature = entityToSignature.at(entity);
		auto archetype = archetypes.at(signature);

		auto collection = std::static_pointer_cast<ComponentCollection<T>>(archetype->components.at(componentTypeName));

		return collection->GetComponent(entity);
	}

	template<typename T>
	std::shared_ptr<T> RegisterSystem() {
		const char* typeName = typeid(T).name();

		assert(systems.find(typeName) == systems.end() && "System already added");

		std::shared_ptr<T> system = std::make_shared<T>();
		systems.insert({ typeName,system });
		systemQueriedArchetype.insert({ typeName, ArchetypeSignature() });

		return system;
	}

	template<typename S, typename C>
	void AddComponentToSystem() {
		const char* systemTypeName = typeid(S).name();
		assert(systems.find(systemTypeName) != systems.end() && "System not registered");

		const char* componentTypeName = typeid(C).name();
		if (componentTypes.find(componentTypeName) == componentTypes.end())
		{
			RegisterComponentType(componentTypeName);
		}

		ComponentTypeID componentTypeID = componentTypes.at(componentTypeName);
		ArchetypeSignature systemArchetypeSignature = systemQueriedArchetype.at(systemTypeName).set(componentTypeID, true);

		UpdateSystemArchetype(systemTypeName, systemArchetypeSignature);
	}


private:
	EntityID nextEntity;

	ComponentTypeID nextComponentType;

	std::unordered_map<EntityID, ArchetypeSignature> entityToSignature;
	std::unordered_map<ArchetypeSignature, std::shared_ptr<Archetype>> archetypes;
	std::unordered_map<const char*, ComponentTypeID> componentTypes;
	std::unordered_map<const char*, std::shared_ptr<System>> systems;
	std::unordered_map<const char*, ArchetypeSignature> systemQueriedArchetype;

	void RegisterComponentType(const char*& typeName);
	ArchetypeSignature UpdateEntitySignature(const EntityID& entity, const ComponentTypeID& componentType, const bool& isSet);
	void CreateNewArchetype(const ArchetypeSignature& newSignature);
	void UpdateSystemArchetype(const char*& systemTypeName, ArchetypeSignature& systemArchetypeID);

	template<typename T>
	void AddComponentToEntityArchetype(const EntityID& entity, T& component, const ComponentTypeID& componentType) {
		ArchetypeSignature oldSignature = entityToSignature.at(entity);
		ArchetypeSignature newSignature = UpdateEntitySignature(entity, componentType, true);

		if (archetypes.find(newSignature) == archetypes.end()) {
			CreateNewArchetype(newSignature);
		}

		std::shared_ptr<Archetype> newArchetype = archetypes.at(newSignature);
		newArchetype->numEntities++;

		if (oldSignature.any()) {
			std::shared_ptr<Archetype> oldArchetype = archetypes.at(oldSignature);
			oldArchetype->numEntities--;
			oldArchetype->TransferComponents(entity, newArchetype);
		}

		newArchetype->AddComponent(entity, component);
	}
};