#pragma once
#include <bitset>

using EntityID = unsigned int;

using ComponentTypeID = unsigned int;
const ComponentTypeID MaxComponents = 16;

using ArchetypeSignature = std::bitset<MaxComponents>;