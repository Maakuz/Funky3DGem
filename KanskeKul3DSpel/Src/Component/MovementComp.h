#pragma once
#include <vector>
#include <unordered_map>

#include "glm/glm.hpp"
#include "Entity.h"

class MovementComp
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

	void addMovement(Entity entity);
	void removeMovement(Entity entity);
	bool hasMovement(Entity entity) { return m_dataMap.count(entity.id); }
	void addAcceleration(Entity entity, glm::vec3 acceleration);

	void applyToTransform(float deltaTime);

	void printImguiDebug(Entity entity);

private:
	std::vector<Movement> m_data;
	std::unordered_map<unsigned int, unsigned int> m_dataMap;
	float m_friction;

	MovementComp();
};