#pragma once
#include <glm/glm.hpp>
#include "Component/Entity.h"

class Camera
{
public:
	Camera(float fov, float screenWidth, float screenHeight);
	virtual ~Camera() { delete m_attachedEntity; }

	void calculateVP();

	glm::mat4 getVP() const { return m_vp; }

	void attachCamera(Entity entity);

	void cameraDebug();

private:
	glm::vec3 m_pos;
	glm::vec3 m_dir;
	glm::vec3 m_up;

	glm::mat4 m_view;
	glm::mat4 m_projection;
	glm::mat4 m_vp;

	Entity* m_attachedEntity;
};