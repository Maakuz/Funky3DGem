#pragma once
#include <vector>
#include <unordered_map>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "System.h"
#include "../ShadowMap.h"


class DirectionalLightComp : System
{
public:
	struct DirectionalLight
	{
		glm::vec3 dir;
		glm::vec3 color;

		DirectionalLight(Entity owner)
		{
			dir = { 0, -1, 0 };
			color = { 1, 1, 1 };

		}
	};

	struct DirLightShadow
	{
		Entity* assignedEntity;
		glm::mat4 projection;
		glm::mat4 view;
		shadowMap map;

		//do NOT use for referencing, people will die
		glm::mat4 vp() const { return projection * view; }

		DirLightShadow():
			map(1024, true)
		{
			assignedEntity = nullptr;
			projection = glm::ortho(-10.f, 10.f, -10.f, 10.f, 0.1f, 100.f);
			view = glm::identity<glm::mat4>();
		}

		virtual ~DirLightShadow()
		{
			delete assignedEntity;
		}
	};

	static DirectionalLightComp& get()
	{
		static DirectionalLightComp instance;
		return instance;
	}

	void addComponent(Entity entity);
	void removeComponent(Entity entity);
	bool hasComponent(Entity entity) const { return m_dataMap.count(entity.id); }
	DirectionalLight getLight(Entity entity) const;

	void setLight(Entity entity, DirectionalLight light);
	void setDir(Entity entity, glm::vec3 dir);
	void setColor(Entity entity, glm::vec3 color);

	void setAsShadowCaster(Entity entity);

	//Calculates view relative to camera
	void calculateView();

	const std::vector<DirectionalLight>* getDirectionalLights() const { return &m_lights; }
	const DirLightShadow* getShadow() const { return &m_shadow; }

	void printImguiDebug(Entity entity);

private:
	std::vector<Entity> m_owner;
	std::vector<DirectionalLight> m_lights;
	DirLightShadow m_shadow;
	std::unordered_map<unsigned int, unsigned int> m_dataMap;
};