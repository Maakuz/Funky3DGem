#pragma once
#include <vector>
#include <deque>
#include "Entity.h"

class EntityManager
{
public:
	EntityManager();
	virtual ~EntityManager() {};

	Entity createEntity();
	void destroyEntity(Entity entity);

	bool isAlive(Entity entity) const;

private:
	std::vector<unsigned char> m_gen;
	std::deque<unsigned int> m_freeElements;

	unsigned int createID(unsigned int index, char gen);
};