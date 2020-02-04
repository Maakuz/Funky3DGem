#include "MovementComp.h"
#include "DataTemplate.h"
#include "TransformComp.h"


MovementComp::MovementComp()
{
    m_friction = 0.8f;
}

void MovementComp::addMovement(Entity entity)
{
    if (!TransformComp::get().hasTransform(entity))
    {
        printfCon("Entity need transform to have movement", entity.id);
        return;
    }

    addData<Movement>(m_dataMap, m_data, entity, Movement(entity));
}

void MovementComp::removeMovement(Entity entity)
{
    removeData<Movement>(m_dataMap, m_data, entity);
}

void MovementComp::addAcceleration(Entity entity, glm::vec3 acceleration)
{
    m_data[m_dataMap[entity.id]].acceleration += acceleration;
}

glm::vec3 MovementComp::getAcceleration(Entity entity) const
{
    return m_data[m_dataMap.at(entity.id)].acceleration;
}

glm::vec3 MovementComp::getVelocity(Entity entity) const
{
    return m_data[m_dataMap.at(entity.id)].velocity;
}

void MovementComp::applyToTransform(float deltaTime)
{
    TransformComp* transform = &TransformComp::get();
    for (Movement& data : m_data)
    {
        data.velocity += data.acceleration * deltaTime;
        data.velocity *= powf(m_friction, deltaTime); //Maybe

        transform->move(data.owner, data.velocity * deltaTime);

        data.acceleration = {0, 0, 0};
    }
}

void MovementComp::printImguiDebug(Entity entity)
{
    using namespace ImGui;

    if (hasMovement(entity))
    {
        glm::vec3 vel = getVelocity(entity);
        glm::vec3 acc = getAcceleration(entity);
        Text("Velocity: %f, %f, %f", vel.x, vel.y, vel.z);
        Text("Acceleration: %f, %f, %f", acc.x, acc.y, acc.z);

        DragFloat("friction", &m_friction, 0.001, 0, 1);

        if (Button(("Test movement " + std::to_string(entity.id)).c_str()))
        {
            addAcceleration(entity, glm::vec3(0, 0, -1));
        }
    }

    else
    {
        if (Button(("Add Movement " + std::to_string(entity.id)).c_str()))
            addMovement(entity);
    }
}