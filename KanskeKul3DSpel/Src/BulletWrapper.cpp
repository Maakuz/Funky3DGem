#include "BulletWrapper.h"

BulletWrapper::BulletWrapper(float gravity)
{
    m_config = new btDefaultCollisionConfiguration();
    m_dispatcher = new btCollisionDispatcher(m_config);
    m_overlappingPairCache = new btDbvtBroadphase();
    m_constraintSolver = new btSequentialImpulseConstraintSolver();

    m_world = new btDiscreteDynamicsWorld(m_dispatcher, m_overlappingPairCache, m_constraintSolver, m_config);
    m_world->setGravity(btVector3(0, -gravity, 0));
}

BulletWrapper::~BulletWrapper()
{
    delete m_world;
    delete m_constraintSolver;
    delete m_overlappingPairCache;
    delete m_dispatcher;
    delete m_config;
}
