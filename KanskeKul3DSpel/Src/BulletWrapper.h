#pragma once
#include "Bullet/btBulletDynamicsCommon.h"

class BulletWrapper
{
public:
	BulletWrapper(float gravity);
	virtual ~BulletWrapper();

private:
	btDefaultCollisionConfiguration* m_config;
	btCollisionDispatcher* m_dispatcher;
	btBroadphaseInterface* m_overlappingPairCache;
	btSequentialImpulseConstraintSolver* m_constraintSolver;

	btDiscreteDynamicsWorld* m_world;
};