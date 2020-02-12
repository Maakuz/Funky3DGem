#include "EntityManager.h"
#include "../Debug/ConsoleWindow.h"

constexpr int MINIMUM_FREE_SLOTS = 100;

EntityManager::EntityManager()
{
}

Entity EntityManager::createEntity()
{
    unsigned int index = 0;
    if (m_freeElements.size() > MINIMUM_FREE_SLOTS || m_gen.size() >= ENTITY_INDEX_MASK)
    {
        //Fill the gap
        printfCon("NOT IMPLEMENTED PLEASE DO IT");
    }

    else if (m_gen.size() < ENTITY_INDEX_MASK)
    {
        m_gen.push_back(0);
        index = m_gen.size() - 1;
    }

    else
    {
        printfCon("Too many entities! Capped at 16M how even is this possible?");
        return Entity(ENTITY_ERROR);
    }

    return Entity(createID(index, m_gen[index]));
}

void EntityManager::destroyEntity(Entity entity)
{
    unsigned int index = entity.index();
    m_gen[index]++;
    m_freeElements.push_back(index);
}

bool EntityManager::isAlive(Entity entity) const
{
    return m_gen[entity.index()] == entity.generation();
}

unsigned int EntityManager::createID(unsigned int index, char gen)
{
    return index | (gen << ENTITY_INDEX_BITS);
}
