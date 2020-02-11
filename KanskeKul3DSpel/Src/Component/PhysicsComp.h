#pragma once
#include <vector>
#include <unordered_map>
#include "System.h"
#include "glm/glm.hpp"
#include "Entity.h"
#include "Bullet/btBulletDynamicsCommon.h"

class PhysicsComp : System
{
public:
	struct Physics
	{
		Entity owner;
		btRigidBody* rigidBody;
		Physics(Entity owner)
		{
			this->owner = owner;
			this->rigidBody = nullptr;
		}
	};

	static PhysicsComp& get()
	{
		static PhysicsComp instance;
		return instance;
	}

	void addComponent(Entity entity);
	void removeComponent(Entity entity);
	bool hasComponent(Entity entity) const { return m_dataMap.count(entity.id); }

	//No need to manage shape outside as it will be deleted when the system or entity dies.
	void setShape(Entity entity, btCollisionShape* shape, float mass);
	void setMass(Entity entity, float mass);
	btRigidBody* getRigidBody(Entity entity);



	void setGravity(float gravity);
	void stepSimulation(float deltaTime);

	void printImguiDebug(Entity entity);

private:
	std::vector<Physics> m_data;
	std::unordered_map<unsigned int, unsigned int> m_dataMap;

	btDefaultCollisionConfiguration* m_config;
	btCollisionDispatcher* m_dispatcher;
	btBroadphaseInterface* m_overlappingPairCache;
	btSequentialImpulseConstraintSolver* m_constraintSolver;

	btDiscreteDynamicsWorld* m_world;

	PhysicsComp();
	~PhysicsComp();
};