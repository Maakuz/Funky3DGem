#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Component/EntityManager.h"
#include "Camera.h"

class Game
{
public:
	Game(GLFWwindow* window);
	virtual ~Game() {};

	void run(float deltaTime);

	const Camera* getCamera() const { return &m_camera; }
private:
	GLFWwindow* m_window;
	EntityManager m_manager;
	Camera m_camera;

	std::vector<Entity> m_entities;
	void debugEntities();
};