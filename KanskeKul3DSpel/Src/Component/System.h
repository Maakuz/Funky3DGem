#pragma once
#include "Entity.h"

class System
{
public:
	virtual void addComponent(Entity entity) = 0;
	virtual void removeComponent(Entity entity) = 0;
	virtual bool hasComponent(Entity entity) const = 0;
	virtual void printImguiDebug(Entity entity) = 0;
};
