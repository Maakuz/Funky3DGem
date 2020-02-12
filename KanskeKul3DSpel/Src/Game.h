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

private:
	struct MouseData
	{
		double x, y;
		double prevX, prevY;

		MouseData()
		{
			x = 0;
			y = 0;
			prevX = 0;
			prevY = 0;
		}
	} m_mouseData;

	GLFWwindow* m_window;
	EntityManager m_manager;

	bool m_lockMouse;
	bool m_debugEntities;
	bool m_consoleVisible;
	bool m_debugCamera;

	static Game* s_instance;
	static GLFWkeyfun s_prevCallback;

	std::vector<Entity> m_entities;

	//Dark places
	static void inputCallbackWrapper(GLFWwindow* window, int key, int code, int action, int mods)
	{
		if (s_prevCallback != NULL)
			s_prevCallback(window, key, code, action, mods);

		s_instance->inputCallback(key, code, action, mods);
	}

	void inputCallback(int key, int code, int action, int mods);
	void debugEntities();
};