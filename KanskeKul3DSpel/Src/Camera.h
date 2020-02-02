#pragma once
#include <glm/glm.hpp>
#include "Component/Entity.h"

class Camera
{
public:
	Camera(float fov, float screenWidth, float screenHeight);
	virtual ~Camera() { delete m_attachedEntity; }

	void trackMouse(float deltaTime, float mouseX, float mouseY);
	void calculateVP();

	glm::mat4 getVP() const { return m_vp; }

	void attachCamera(Entity entity);

	void cameraDebug(bool* isOpen);

private:
	glm::vec3 m_pos;
	glm::vec3 m_forward;
	glm::vec3 m_up;
	glm::vec3 m_right;

	glm::mat4 m_view;
	glm::mat4 m_projection;
	glm::mat4 m_vp;

	int m_width;
	int m_height;
	float m_mouseSpeed;

	Entity* m_attachedEntity;
};