#include "LightComp.h"
#include "DataTemplate.h"


void LightComp::addComponent(Entity entity)
{
    addData<DirectionalLight>(m_dataMap, m_data, entity, DirectionalLight(entity));
}

void LightComp::removeComponent(Entity entity)
{
    removeData<DirectionalLight>(m_dataMap, m_data, entity);
}

LightComp::DirectionalLight LightComp::getLight(Entity entity) const
{
    if (!m_dataMap.count(entity.id))
    {
        printfCon("Entity %d has no light", entity.id);
        return DirectionalLight(ENTITY_ERROR);
    }

    return m_data[m_dataMap.at(entity.id)];
}

void LightComp::setLight(Entity entity, DirectionalLight light)
{
    if (!m_dataMap.count(entity.id))
    {
        printfCon("Entity %d has no light", entity.id);
        return;
    }

    m_data[m_dataMap.at(entity.id)] = light;
}

void LightComp::setDir(Entity entity, glm::vec3 dir)
{
    if (!m_dataMap.count(entity.id))
    {
        printfCon("Entity %d has no light", entity.id);
        return;
    }

    m_data[m_dataMap.at(entity.id)].dir = dir;
}

void LightComp::setColor(Entity entity, glm::vec3 color)
{
    if (!m_dataMap.count(entity.id))
    {
        printfCon("Entity %d has no light", entity.id);
        return;
    }

    m_data[m_dataMap.at(entity.id)].color = color;
}

void LightComp::printImguiDebug(Entity entity)
{
    using namespace ImGui;

    if (hasComponent(entity))
    {
        DragFloat3(("Direction " + std::to_string(entity.id)).c_str(), &m_data[m_dataMap[entity.id]].dir.x);
        DragFloat3(("Color " + std::to_string(entity.id)).c_str(), &m_data[m_dataMap[entity.id]].color.x);

        if (Button(("Normalize dir " + std::to_string(entity.id)).c_str()))
            m_data[m_dataMap[entity.id]].dir = glm::normalize(m_data[m_dataMap[entity.id]].dir);
    }

    else
    {
        if (Button(("Add light " + std::to_string(entity.id)).c_str()))
            addComponent(entity);
    }

}



