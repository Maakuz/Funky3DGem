#include "PhysicsComp.h"
#include <GL/glew.h>
#include <filesystem>
#include "DataTemplate.h"
#include "TransformComp.h"
#include "Importer/Importer.h"

constexpr const char* MODEL_NAMES[2] = { "Cube", "Sphere" };

void PhysicsComp::addComponent(Entity entity)
{
    if (!TransformComp::get().hasComponent(entity))
    {
        printfCon("Entity need transform to have a model", entity.id);
        return;
    }


    addData<Physics>(m_dataMap, m_data, entity, Physics(entity));
}

void PhysicsComp::removeComponent(Entity entity)
{
    removeData<Physics>(m_dataMap, m_data, entity);
}

void PhysicsComp::printImguiDebug(Entity entity)
{
    using namespace ImGui;
    if (hasComponent(entity))
    {
        
    }

    else
    {
        
    }
}

