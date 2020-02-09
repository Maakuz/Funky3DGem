#include "LightComp.h"
#include "DataTemplate.h"


void LightComp::addComponent(Entity entity)
{
    addData<DirectionalLight>(m_dataMap, m_data, entity, DirectionalLight(entity));
    m_owner.push_back(entity);
}

void LightComp::removeComponent(Entity entity)
{
    if (!m_dataMap.count(entity.id))
    {
        printfCon("Entity %d does not have this component", entity.id);
        return;
    }

    Entity last = m_owner.back();
    unsigned int index = m_dataMap[entity.id];

    unordered_erase(m_data, m_data.begin() + index);
    m_dataMap[last.id] = index;
    m_dataMap.erase(entity.id);

    m_owner[entity.id] = m_owner.back();
    m_owner.pop_back();
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
        DragFloat3(("Direction " + std::to_string(entity.id)).c_str(), &m_data[m_dataMap[entity.id]].dir.x, 0.01, -1, 1);
        DragFloat3(("Color " + std::to_string(entity.id)).c_str(), &m_data[m_dataMap[entity.id]].color.x, 0.01, 0, 1);

        m_data[m_dataMap[entity.id]].dir = glm::normalize(m_data[m_dataMap[entity.id]].dir);
    }

    else
    {
        if (Button(("Add light " + std::to_string(entity.id)).c_str()))
            addComponent(entity);
    }

}



