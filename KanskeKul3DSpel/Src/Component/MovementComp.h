#pragma once
#include <vector>
#include <unordered_map>
#include "System.h"
#include "glm/glm.hpp"
#include "Entity.h"

class MovementComp : System
{
public:
	struct Movement
	{
		Entity owner;
		glm::vec3 velocity;
		glm::vec3 acceleration;

		Movement(Entity owner)
		{
			this->owner = owner;
			this->acceleration = { 0, 0, 0 };
			this->velocity = { 0, 0, 0 };
		}
	};

	static MovementComp& get()
	{
		static MovementComp instance;
		return instance;
	}

	void addComponent(Entity entity);
	void removeComponent(Entity entity);
	bool hasComponent(Entity entity) const { return m_dataMap.count(entity.id); }
	void addAcceleration(Entity entity, glm::vec3 acceleration);

	glm::vec3 getAcceleration(Entity entity) const;
	glm::vec3 getVelocity(Entity entity) const;

	void applyToTransform(float deltaTime);

	void printImguiDebug(Entity entity);

private:
	std::vector<Movement> m_data;
	std::unordered_map<unsigned int, unsigned int> m_dataMap;
	float m_friction;

	MovementComp();
};