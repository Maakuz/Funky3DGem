#include "PhysicsComp.h"
#include <GL/glew.h>
#include "DataTemplate.h"
#include "TransformComp.h"
#include "../BulletWrapper/BulletGlmConversion.h"

PhysicsComp::PhysicsComp()
{
    m_config = new btDefaultCollisionConfiguration();
    m_dispatcher = new btCollisionDispatcher(m_config);
    m_overlappingPairCache = new btDbvtBroadphase();
    m_constraintSolver = new btSequentialImpulseConstraintSolver();

    m_world = new btDiscreteDynamicsWorld(m_dispatcher, m_overlappingPairCache, m_constraintSolver, m_config);
    setGravity(1);
}

PhysicsComp::~PhysicsComp()
{
    //remove the rigidbodies from the dynamics world and delete them
    for (int i = m_world->getNumCollisionObjects() - 1; i >= 0; i--)
    {
        btCollisionObject* obj = m_world->getCollisionObjectArray()[i];
        btRigidBody* body = btRigidBody::upcast(obj);
        if (body && body->getMotionState())
        {
            delete body->getCollisionShape();
            delete body->getMotionState();
        }
        m_world->removeCollisionObject(obj);
        delete obj;
    }

    delete m_world;
    delete m_constraintSolver;
    delete m_overlappingPairCache;
    delete m_dispatcher;
    delete m_config;
}

void PhysicsComp::addComponent(Entity entity)
{
    if (!TransformComp::get().hasComponent(entity))
    {
        printfCon("Entity need transform to have physics", entity.id);
        return;
    }


    addData<Physics>(m_dataMap, m_data, entity, Physics(entity));

    setShape(entity, new btBoxShape(btVector3(btScalar(0.5), btScalar(0.5), btScalar(0.5))), 10);
}

void PhysicsComp::removeComponent(Entity entity)
{
    removeData<Physics>(m_dataMap, m_data, entity);
}

void PhysicsComp::setShape(Entity entity, btCollisionShape* shape, float mass)
{
    if (!hasComponent(entity))
    {
        printfCon("Entity has no physics", entity.id);
        if (shape)
            delete shape;

        return;
    }

    TransformComp* transform = &TransformComp::get();

    btRigidBody*& currentBody = m_data[m_dataMap[entity.id]].rigidBody;

    if (currentBody)
    {
        m_world->removeRigidBody(currentBody);
        delete currentBody;
    }

    btTransform groundTransform;
    groundTransform.setIdentity();
    groundTransform.setOrigin(glmToBullet(transform->getPosition(entity)));
    groundTransform.setRotation(glmToBullet(transform->getRotation(entity)));

    btVector3 localInertia(0, 0, 0);
    if (abs(mass) > FLT_EPSILON)
        shape->calculateLocalInertia(mass, localInertia);

    btDefaultMotionState* motionState = new btDefaultMotionState(groundTransform);
    btRigidBody::btRigidBodyConstructionInfo info(mass, motionState, shape, localInertia);

    currentBody = new btRigidBody(info);
    m_world->addRigidBody(currentBody);
}

void PhysicsComp::setGravity(float gravity)
{
    m_world->setGravity(btVector3(0, -gravity, 0));
}

void PhysicsComp::stepSimulation(float deltaTime)
{
    m_world->stepSimulation(deltaTime, 10);
    
    TransformComp* transformComp = &TransformComp::get();
    for (Physics& data : m_data)
    {
       btTransform* transform = &data.rigidBody->getWorldTransform();

       transformComp->setPosition(data.owner, bulletToGlm(transform->getOrigin()));
       transformComp->setRotation(data.owner, bulletToGlm(transform->getRotation()));
    }
}

void PhysicsComp::printImguiDebug(Entity entity)
{
    using namespace ImGui;
    if (hasComponent(entity))
    {
        
    }

    else
    {
        if (Button(("Add transform" + std::to_string(entity.id)).c_str()))
            addComponent(entity);
    }
}

