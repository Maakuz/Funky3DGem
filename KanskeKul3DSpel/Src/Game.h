#pragma once
#include <GLFW/glfw3.h>
#include "Component/EntityManager.h"


class Game
{
public:
	Game(GLFWwindow* window);
	virtual ~Game() {};

	void run(float deltaTime);

private:
	GLFWwindow* m_window;
	EntityManager m_manager;

	Entity m_player;
};