#pragma once
#include <unordered_map>
#include <vector>
#include "Entity.h"
#include "../Debug/ConsoleWindow.h"
#include "../UnorderedErase.h"

template <typename DataStruct>
inline void addData(std::unordered_map<unsigned int, unsigned int>& map, std::vector<DataStruct>& data, Entity entity, DataStruct component)
{
    if (map.count(entity.id))
    {
        printfCon("Entity %d has component", entity.id);
        return;
    }

    unsigned int index = data.size();

    data.push_back(component);

    map.emplace(entity.id, index);
}

template <typename DataStruct>
inline void removeData(std::unordered_map<unsigned int, unsigned int>& map, std::vector<DataStruct>& data, Entity entity)
{
    if (!map.count(entity.id))
    {
        printfCon("Entity %d does not have this component", entity.id);
        return;
    }

    Entity last = data.back().owner;
    unsigned int index = map[entity.id];

    unordered_erase(data, data.begin() + index);
    map[last.id] = index;
    map.erase(entity.id);
}