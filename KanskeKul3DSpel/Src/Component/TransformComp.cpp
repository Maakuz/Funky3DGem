#include "TransformComp.h"
#include "../UnorderedErase.h"
#include "../ConsoleWindow.h"

void TransformComp::addTransform(Entity entity)
{
    if (m_dataMap.count(entity.id))
    {
        printfCon("Entity %d has already a tranform", entity.id);
        return;
    }
    unsigned int index = m_data.size();

    m_data.push_back(Transform(glm::vec3(0, 0, 0), entity));

    m_dataMap.emplace(entity.id, index);
}

void TransformComp::removeTransform(Entity entity)
{
    if (!m_dataMap.count(entity.id))
    {
        printfCon("Entity %d has no tranform", entity.id);
        return;
    }

    Entity last = m_data.back().owner;
    unsigned int index = m_dataMap[entity.id];

    unordered_erase(m_data, m_data.begin() + index);
    m_dataMap[last.id] = index;
    m_dataMap.erase(entity.id);
}

glm::mat4 TransformComp::getTransformMat(Entity entity)
{
    return glm::mat4();
}

glm::vec3 TransformComp::getPosition(Entity entity)
{
    if (!m_dataMap.count(entity.id))
    {
        printfCon("Entity %d has no tranform", entity.id);
        return glm::vec3();
    }

    return m_data[m_dataMap[entity.id]].pos;
}
