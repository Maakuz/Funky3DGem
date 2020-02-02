#include "LightComp.h"
#include "DataTemplate.h"


void LightComp::addLight(Entity entity)
{
    addData<DirectionalLight>(m_dataMap, m_data, entity, DirectionalLight(entity));
}

void LightComp::removeLight(Entity entity)
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



