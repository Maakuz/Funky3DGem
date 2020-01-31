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
	glm::mat4 getTransformMat(Entity entity);
	glm::vec3 getPosition(Entity entity);

private:
	struct Transform
	{
		Entity owner;
		glm::vec3 pos;
		//rotation
		//scale

		Transform(glm::vec3 pos, Entity owner)
		{
			this->pos = pos;
			this->owner = owner;
		}
	};

	std::vector<Transform> m_data;
	std::unordered_map<unsigned int, unsigned int> m_dataMap;
};