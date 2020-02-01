#include "TransformComp.h"
#include "DataTemplate.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtx/quaternion.hpp"

void TransformComp::addTransform(Entity entity)
{
    addData<Transform>(m_dataMap, m_data, entity, Transform(entity));
}

void TransformComp::removeTransform(Entity entity)
{
    removeData(m_dataMap, m_data, entity);
}

glm::mat4 TransformComp::getTransformMat(Entity entity) const
{
    Transform data = m_data[m_dataMap.at(entity.id)];

    glm::mat4 translate = glm::translate(glm::identity<glm::mat4>(), data.pos);

    glm::mat4 scale = glm::scale(glm::identity<glm::mat4>(), data.scale);

    
    glm::quat quat(data.rotation);

    glm::mat4 rotation = glm::toMat4(quat);

    return translate * rotation * scale;
}

glm::vec3 TransformComp::getPosition(Entity entity) const
{
    if (!m_dataMap.count(entity.id))
    {
        printfCon("Entity %d has no tranform", entity.id);
        return glm::vec3();
    }

    return m_data[m_dataMap.at(entity.id)].pos;
}

void TransformComp::setPosition(Entity entity, glm::vec3 pos)
{
    if (!m_dataMap.count(entity.id))
    {
        printfCon("Entity %d has no tranform", entity.id);
        return;
    }

    m_data[m_dataMap[entity.id]].pos = pos;
}

void TransformComp::move(Entity entity, glm::vec3 offset)
{
    if (!m_dataMap.count(entity.id))
    {
        printfCon("Entity %d has no tranform", entity.id);
        return;
    }

    m_data[m_dataMap[entity.id]].pos += offset;
}

glm::vec3 TransformComp::getScale(Entity entity) const
{
    if (!m_dataMap.count(entity.id))
    {
        printfCon("Entity %d has no tranform", entity.id);
        return glm::vec3();
    }

    return m_data[m_dataMap.at(entity.id)].scale;
}

void TransformComp::setScale(Entity entity, glm::vec3 scale)
{
    if (!m_dataMap.count(entity.id))
    {
        printfCon("Entity %d has no tranform", entity.id);
        return;
    }

    m_data[m_dataMap[entity.id]].scale = scale;
}

glm::vec3 TransformComp::getRotation(Entity entity) const
{
    if (!m_dataMap.count(entity.id))
    {
        printfCon("Entity %d has no tranform", entity.id);
        return glm::vec3();
    }

    return m_data[m_dataMap.at(entity.id)].rotation;
}

void TransformComp::setRotation(Entity entity, glm::vec3 rotation)
{
    if (!m_dataMap.count(entity.id))
    {
        printfCon("Entity %d has no tranform", entity.id);
        return;
    }

    m_data[m_dataMap[entity.id]].rotation = rotation;
}