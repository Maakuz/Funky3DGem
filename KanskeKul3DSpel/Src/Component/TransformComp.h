#pragma once
#include <vector>
#include <unordered_map>
#include "glm/glm.hpp"
#include "Entity.h"

class TransformComp
{
public:
	static TransformComp& get() 
	{
		static TransformComp instance;
		return instance;
	}

	void addTransform(Entity entity);
	void removeTransform(Entity entity);
	bool hasTransform(Entity entity) { return m_dataMap.count(entity.id); }
	glm::mat4 getTransformMat(Entity entity)  const;
	glm::vec3 getPosition(Entity entity) const;
	void setPosition(Entity entity, glm::vec3 pos);
	glm::vec3 getScale(Entity entity) const;
	void setScale(Entity entity, glm::vec3 scale);
	glm::vec3 getRotation(Entity entity) const;
	void setRotation(Entity entity, glm::vec3 rotation);
	void move(Entity entity, glm::vec3 offset);

	void printImguiDebug(Entity entity);

private:
	struct Transform
	{
		Entity owner;
		glm::vec3 pos;
		glm::vec3 scale;
		glm::vec3 rotation;

		Transform(Entity owner)
		{
			this->owner = owner;
			this->scale = {1, 1, 1};
			this->pos = {0, 0, 0};
			this->rotation = pos;
		}
	};

	std::vector<Transform> m_data;
	std::unordered_map<unsigned int, unsigned int> m_dataMap;
};