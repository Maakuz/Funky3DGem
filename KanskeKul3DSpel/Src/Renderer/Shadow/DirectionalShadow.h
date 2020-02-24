#pragma once
#include "Component/Entity.h"
#include "ShadowMap.h"
#include "glm/glm.hpp"

class DirectionalShadow
{
public:
	DirectionalShadow();
	virtual ~DirectionalShadow() {}

	//do NOT use for referencing, people will die
	void calculateVP();

	const glm::mat4& getVP() const { return m_vp; }

	void assignEntity(Entity* entity);
	const Entity* getAssignedEntity() const { return m_assignedEntity; }
	const ShadowMap* getMap() const { return &m_map; }

	void setView(glm::mat4 view);
	const glm::mat4& getView() const { return m_view; }
private:
	Entity* m_assignedEntity;
	ShadowMap m_map;

	bool m_dirty;
	glm::mat4 m_projection;
	glm::mat4 m_view;
	glm::mat4 m_vp;
};
