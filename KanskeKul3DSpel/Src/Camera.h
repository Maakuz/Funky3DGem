#pragma once
#include <glm/glm.hpp>


class Camera
{
public:
	Camera(float fov, float screenWidth, float screenHeight);
	virtual ~Camera() {};

	glm::mat4 getVP() const { return m_vp; }

private:
	glm::vec3 m_pos;
	glm::vec3 m_dir;
	glm::vec3 m_up;

	glm::mat4 m_view;
	glm::mat4 m_projection;
	glm::mat4 m_vp;
};