#pragma once
#include <bitset>

using EntityID = unsigned int;

using ComponentTypeID = unsigned int;
const ComponentTypeID MaxComponents = 16;

using ArchetypeSignature = std::bitset<MaxComponents>;

inline bool IsSubset(const ArchetypeSignature& archetype, const ArchetypeSignature& queryMask) {
	return (archetype & queryMask) == queryMask;
}