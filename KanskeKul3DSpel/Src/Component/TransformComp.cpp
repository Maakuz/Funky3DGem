#include "TransformComp.h"
#include "PhysicsComp.h"
#include "DataTemplate.h"
#include "../BulletWrapper/BulletGlmConversion.h"

void TransformComp::addComponent(Entity entity)
{
    addData<Transform>(m_dataMap, m_data, entity, Transform(entity));
}

void TransformComp::removeComponent(Entity entity)
{
    removeData(m_dataMap, m_data, entity);
}

glm::mat4 TransformComp::getTransformMat(Entity entity)
{
    if (!m_dataMap.count(entity.id))
    {
        printfCon("Entity %d has no tranform", entity.id);
        return glm::identity<glm::mat4>();
    }

    computeMat(entity);

    return m_data[m_dataMap.at(entity.id)].transform;
}

glm::mat4 TransformComp::getInverseTransposeMat(Entity entity)
{
    if (!m_dataMap.count(entity.id))
    {
        printfCon("Entity %d has no tranform", entity.id);
        return glm::identity<glm::mat4>();
    }

    computeMat(entity);

    return m_data[m_dataMap.at(entity.id)].invTranspose;
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

    if (PhysicsComp::get().hasComponent(entity))
        PhysicsComp::get().getRigidBody(entity)->getWorldTransform().setOrigin(glmToBullet(pos));
}

void TransformComp::move(Entity entity, glm::vec3 offset)
{
    if (!m_dataMap.count(entity.id))
    {
        printfCon("Entity %d has no tranform", entity.id);
        return;
    }

    glm::vec3 newPos = m_data[m_dataMap[entity.id]].pos += offset;
    m_data[m_dataMap[entity.id]].dirty = true;

    if (PhysicsComp::get().hasComponent(entity))
        PhysicsComp::get().getRigidBody(entity)->getWorldTransform().setOrigin(glmToBullet(newPos));
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

    if (PhysicsComp::get().hasComponent(entity))
        PhysicsComp::get().getRigidBody(entity)->getCollisionShape()->setLocalScaling(glmToBullet(scale));
}

glm::quat TransformComp::getRotation(Entity entity) const
{
    if (!m_dataMap.count(entity.id))
    {
        printfCon("Entity %d has no tranform", entity.id);
        return glm::vec3();
    }

    return m_data[m_dataMap.at(entity.id)].rotation;
}

void TransformComp::setRotation(Entity entity, glm::quat rotation)
{
    if (!m_dataMap.count(entity.id))
    {
        printfCon("Entity %d has no tranform", entity.id);
        return;
    }

    m_data[m_dataMap[entity.id]].rotation = rotation;
    m_data[m_dataMap[entity.id]].dirty = true;

    if (PhysicsComp::get().hasComponent(entity))
        PhysicsComp::get().getRigidBody(entity)->getWorldTransform().setRotation(glmToBullet(rotation));
}

glm::vec3 TransformComp::getRight(Entity entity)
{
    glm::mat4 transform = getTransformMat(entity);
    return glm::normalize(glm::vec3(transform[0][0], transform[0][1], transform[0][2]));
}

glm::vec3 TransformComp::getForward(Entity entity)
{
    glm::mat4 transform = getTransformMat(entity);
    return glm::normalize(glm::vec3(transform[2][0], transform[2][1], transform[2][2]));
}

glm::vec3 TransformComp::getUp(Entity entity)
{
    glm::mat4 transform = getTransformMat(entity);
    return glm::normalize(glm::vec3(transform[1][0], transform[1][1], transform[1][2]));
}


void TransformComp::printImguiDebug(Entity entity)
{
    using namespace ImGui;

    if (hasComponent(entity))
    {
        Text("Transform");
        glm::vec3 pos = getPosition(entity);
        DragFloat3(("Position " + std::to_string(entity.id)).c_str(), &pos[0], 0.5);
        setPosition(entity, pos);

        glm::vec3 scale = getScale(entity);
        DragFloat3(("Scale " + std::to_string(entity.id)).c_str(), &scale[0], 0.5);
        setScale(entity, scale);

        static float ypr[3]{0, 0, 0}; //Yawpitchroll
        float prevYpr[3] = { ypr[0], ypr[1], ypr[2] };

        DragFloat3(("Rotation " + std::to_string(entity.id)).c_str(), ypr, 0.01);
        glm::quat rotation = glm::quat(glm::vec3(ypr[0] - prevYpr[0], ypr[1] - prevYpr[1], ypr[2] - prevYpr[2]));

        glm::quat rot = getRotation(entity);
        rot = rotation * rot;
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

        if (TreeNode(("normal matrix " + std::to_string(entity.id)).c_str()))
        {
            glm::mat4 transformMat = getInverseTransposeMat(entity);
            for (int j = 0; j < 4; j++)
            {
                Columns(4);

                for (int k = 0; k < 4; k++)
                {
                    Text("%f", transformMat[j][k]);
                    NextColumn();
                }
                setRotation(entity, glm::vec3());
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
            addComponent(entity);
    }
}

void TransformComp::computeMat(Entity entity)
{
    Transform& data = m_data[m_dataMap[entity.id]];

    if (data.dirty)
    {
        glm::mat4 translate = glm::translate(glm::identity<glm::mat4>(), data.pos);

        glm::mat4 scale = glm::scale(glm::identity<glm::mat4>(), data.scale);

        glm::mat4 rotation = glm::toMat4(data.rotation);

        data.transform = translate * rotation * scale;
        data.invTranspose = glm::transpose(glm::inverse(data.transform));
        data.dirty = false;
    }
}
