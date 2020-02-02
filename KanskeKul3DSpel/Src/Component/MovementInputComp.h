#pragma once
#include <vector>
#include <unordered_map>

#include "GLFW/glfw3.h"
#include "Entity.h"

class MovementInputComp
{
public:
	struct Keys 
	{
		unsigned int forward;
		unsigned int backward;
		unsigned int strafeLeft;
		unsigned int strafeRight;
	};

	static MovementInputComp& get()
	{
		static MovementInputComp instance;
		return instance;
	}

	void addInput(Entity entity);
	void removeInput(Entity entity);
	Keys getKeys(Entity entity);
	void setKeys(Entity entity, Keys keys);

private:
	std::vector<Keys> m_data;
	std::unordered_map<unsigned int, unsigned int> m_dataMap;
};