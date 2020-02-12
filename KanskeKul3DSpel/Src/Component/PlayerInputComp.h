#pragma once
#include <vector>
#include <unordered_map>

#include "GLFW/glfw3.h"
#include "Entity.h"

class PlayerInputComp
{
public:
	struct Keys 
	{
		Entity owner;
		unsigned int forward;
		unsigned int backward;
		unsigned int strafeLeft;
		unsigned int strafeRight;
		float movementSpeed;

		Keys(Entity owner)
		{
			this->owner = owner;
			this->forward = GLFW_KEY_W;
			this->backward = GLFW_KEY_S;
			this->strafeLeft = GLFW_KEY_A;
			this->strafeRight = GLFW_KEY_D;
			this->movementSpeed = 10;
		}
	};

	static PlayerInputComp& get()
	{
		static PlayerInputComp instance;
		return instance;
	}

	void addInput(Entity entity);
	void removeInput(Entity entity);
	bool hasInput(Entity entity) const { return m_dataMap.count(entity.id); }
	Keys getKeys(Entity entity) const { return m_data[m_dataMap.at(entity.id)]; }
	void setKeys(Entity entity, Keys keys);

	void handleInputs(GLFWwindow* window);

	void printImguiDebug(Entity entity);

private:
	std::vector<Keys> m_data;
	std::unordered_map<unsigned int, unsigned int> m_dataMap;
};