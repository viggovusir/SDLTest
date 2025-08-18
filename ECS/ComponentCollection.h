#include "Types.h"
#include <memory>
#include <vector>
#include <unordered_map>
#include <cassert>

class IComponentCollection {
public:
	virtual ~IComponentCollection() = default;
	virtual void RemoveComponent(const EntityID& entity) = 0;
	virtual void TransferComponents(const EntityID& entity, std::shared_ptr<IComponentCollection> newCollection) = 0;
	virtual void EntityDestroyed(const EntityID& entity) = 0;
	virtual std::shared_ptr<IComponentCollection> CreateEmptyClone() const = 0;
};

template<typename T>
class ComponentCollection : public IComponentCollection {
public:
	void AddComponent(const EntityID& entity, const T& component) {
		assert(entityToComponentIndex.find(entity) == entityToComponentIndex.end() && "Component type already added to entity");

		size_t index = components.size();
		components.push_back(component);
		entityToComponentIndex.insert({ entity, index });
		componentIndexToEntity.insert({ index, entity });
	}

	void RemoveComponent(const EntityID& entity) {
		assert(entityToComponentIndex.find(entity) != entityToComponentIndex.end() && "Component does not exist on entity");

		size_t removedEntityIndex = entityToComponentIndex.at(entity);
		size_t lastIndex = entityToComponentIndex.size() - 1;

		components[removedEntityIndex] = components[lastIndex];
		components.pop_back();

		EntityID lastEntity = componentIndexToEntity[lastIndex];
		entityToComponentIndex.at(lastEntity) = removedEntityIndex;
		componentIndexToEntity.at(removedEntityIndex) = lastEntity;

		entityToComponentIndex.erase(entity);
		componentIndexToEntity.erase(lastIndex);
	}

	std::vector<T>& GetComponents() {
		return components;
	}

	T& GetComponent(const EntityID& entity) {
		assert(entityToComponentIndex.find(entity) != entityToComponentIndex.end() && "Component does not exist on entity");

		return components[entityToComponentIndex.at(entity)];
	}

	void EntityDestroyed(const EntityID& entity) {
		if (entityToComponentIndex.find(entity) != entityToComponentIndex.end()) {
			RemoveComponent(entity);
		}
	}

	std::shared_ptr<IComponentCollection> CreateEmptyClone() const {
		return std::make_shared<ComponentCollection<T>>();
	}

	void TransferComponents(const EntityID& entity, std::shared_ptr<IComponentCollection> newCollection) {
		std::shared_ptr<ComponentCollection<T>> newPtr = std::static_pointer_cast<ComponentCollection<T>>(newCollection);
		newPtr->AddComponent(entity, this->GetComponent(entity));
	}

private:
	std::vector<T> components;

	std::unordered_map<EntityID, size_t> entityToComponentIndex;
	std::unordered_map<size_t, EntityID> componentIndexToEntity;
};