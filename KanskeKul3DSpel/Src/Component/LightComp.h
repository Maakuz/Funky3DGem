#pragma once
#include <vector>
#include <unordered_map>
#include "glm/glm.hpp"
#include "System.h"


class LightComp : System
{
public:
	struct DirectionalLight
	{
		Entity owner;
		glm::vec3 dir;
		glm::vec3 color;

		DirectionalLight(Entity owner)
		{
			this->owner = owner;
			dir = {0, -1, 0};
			color = {1, 1, 1};
		}
	};

	static LightComp& get()
	{
		static LightComp instance;
		return instance;
	}

	void addComponent(Entity entity);
	void removeComponent(Entity entity);
	bool hasComponent(Entity entity) { return m_dataMap.count(entity.id); }
	DirectionalLight getLight(Entity entity) const;

	void setLight(Entity entity, DirectionalLight light);
	void setDir(Entity entity, glm::vec3 dir);
	void setColor(Entity entity, glm::vec3 color);

	void printImguiDebug(Entity entity);

private:
	std::vector<DirectionalLight> m_data;
	std::unordered_map<unsigned int, unsigned int> m_dataMap;
};