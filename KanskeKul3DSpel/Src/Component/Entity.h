#pragma once
constexpr unsigned int ENTITY_INDEX_BITS = 24;
constexpr unsigned int ENTITY_INDEX_MASK = (1 << ENTITY_INDEX_BITS) - 1;
constexpr unsigned int ENTITY_ERROR = ENTITY_INDEX_MASK;

struct Entity
{
    unsigned int id;

    Entity(unsigned int id) { this->id = id; }

    unsigned int index() { return id & ENTITY_INDEX_MASK; }
    unsigned int generation() { return id >> ENTITY_INDEX_BITS; }
};