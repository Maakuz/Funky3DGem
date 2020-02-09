#pragma once
#include <vector>
#include <unordered_map>
#include "System.h"
#include "glm/glm.hpp"
#include "Entity.h"

class PhysicsComp : System
{
public:
	struct Physics
	{
		Entity owner;

		Physics(Entity owner)
		{
			this->owner = owner;
		}
	};

	static PhysicsComp& get()
	{
		static PhysicsComp instance;
		return instance;
	}

	void addComponent(Entity entity);
	void removeComponent(Entity entity);
	bool hasComponent(Entity entity) const { return m_dataMap.count(entity.id); }

	void printImguiDebug(Entity entity);

private:
	std::vector<Physics> m_data;
	std::unordered_map<unsigned int, unsigned int> m_dataMap;
	float m_friction;
};