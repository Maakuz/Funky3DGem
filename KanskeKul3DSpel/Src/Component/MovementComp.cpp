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

void MovementComp::applyToTransform(float deltaTime)
{
    TransformComp* transform = &TransformComp::get();
    for (Movement& data : m_data)
    {
        data.velocity += data.acceleration * deltaTime;

        transform->move(data.owner, data.velocity * deltaTime);
    }
}

void MovementComp::printImguiDebug(Entity entity)
{
    using namespace ImGui;

    if (hasMovement(entity))
    {

        DragFloat("friction", &m_friction, 0.001, 0, 1);
    }

    else
    {
        if (Button(("Add Movement " + std::to_string(entity.id)).c_str()))
            addMovement(entity);
    }
}