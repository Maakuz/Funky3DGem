#include "TransformComp.h"
#include "DataTemplate.h"
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

    if (data.dirty)
    {
        glm::mat4 translate = glm::translate(glm::identity<glm::mat4>(), data.pos);

        glm::mat4 scale = glm::scale(glm::identity<glm::mat4>(), data.scale);


        glm::quat quat(data.rotation);

        glm::mat4 rotation = glm::toMat4(quat);

        data.transform = translate * rotation * scale;
        data.dirty = false;
    }

    return data.transform;
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
    m_data[m_dataMap[entity.id]].dirty = true;
}

void TransformComp::move(Entity entity, glm::vec3 offset)
{
    if (!m_dataMap.count(entity.id))
    {
        printfCon("Entity %d has no tranform", entity.id);
        return;
    }

    m_data[m_dataMap[entity.id]].pos += offset;
    m_data[m_dataMap[entity.id]].dirty = true;
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
    m_data[m_dataMap[entity.id]].dirty = true;
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
    m_data[m_dataMap[entity.id]].dirty = true;
}

glm::vec3 TransformComp::getRight(Entity entity) const
{
    glm::mat4 transform = getTransformMat(entity);
    return glm::normalize(glm::vec3(transform[0][0], transform[0][1], transform[0][2]));
}

glm::vec3 TransformComp::getForward(Entity entity) const
{
    glm::mat4 transform = getTransformMat(entity);
    return glm::normalize(glm::vec3(transform[2][0], transform[2][1], transform[2][2]));
}

glm::vec3 TransformComp::getUp(Entity entity) const
{
    glm::mat4 transform = getTransformMat(entity);
    return glm::normalize(glm::vec3(transform[1][0], transform[1][1], transform[1][2]));
}


void TransformComp::printImguiDebug(Entity entity)
{
    using namespace ImGui;

    if (hasTransform(entity))
    {
        Text("Transform");
        glm::vec3 pos = getPosition(entity);
        DragFloat3(("Position " + std::to_string(entity.id)).c_str(), &pos[0], 0.5);
        setPosition(entity, pos);

        glm::vec3 scale = getScale(entity);
        DragFloat3(("Scale " + std::to_string(entity.id)).c_str(), &scale[0], 0.5);
        setScale(entity, scale);

        glm::vec3 rot = getRotation(entity);
        DragFloat3(("Rotation " + std::to_string(entity.id)).c_str(), &rot[0], 0.1);
        setRotation(entity, rot);

        if (TreeNode(("matrix " + std::to_string(entity.id)).c_str()))
        {
            glm::mat4 transformMat = getTransformMat(entity);
            for (int j = 0; j < 4; j++)
            {
                Columns(4);

                for (int k = 0; k < 4; k++)
                {
                    Text("%f", transformMat[j][k]);
                    NextColumn();
                }

            }

            Columns(1);
            TreePop();
        }

        glm::vec3 right = getRight(entity);
        glm::vec3 up = getUp(entity);
        glm::vec3 forward = getForward(entity);
        Text("Right: %f, %f, %f", right.x, right.y, right.z);
        Text("Up: %f, %f, %f", up.x, up.y, up.z);
        Text("Forward: %f, %f, %f", forward.x, forward.y, forward.z);

    }

    else
    {
        if (Button(("Add transform" + std::to_string(entity.id)).c_str()))
            addTransform(entity);
    }
}
