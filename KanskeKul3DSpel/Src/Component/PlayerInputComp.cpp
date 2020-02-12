#include "PlayerInputComp.h"
#include "DataTemplate.h"
#include "PhysicsComp.h"
#include "TransformComp.h"
#include "../BulletWrapper/BulletGlmConversion.h"
#include "../Camera.h"

void PlayerInputComp::addInput(Entity entity)
{
    if (!PhysicsComp::get().hasComponent(entity))
    {
        printfCon("Entity need transform to have movement", entity.id);
        return;
    }

    addData<Keys>(m_dataMap, m_data, entity, Keys(entity));
}

void PlayerInputComp::removeInput(Entity entity)
{
    removeData<Keys>(m_dataMap, m_data, entity);
}

void PlayerInputComp::setKeys(Entity entity, Keys keys)
{
    m_data[m_dataMap[entity.id]] = keys;
}

void PlayerInputComp::handleInputs(GLFWwindow* window)
{
    TransformComp* transform = &TransformComp::get();
    PhysicsComp* physics = &PhysicsComp::get();
    for (Keys& data : m_data)
    {
        glm::vec3 forward;
        glm::vec3 right;
        Camera* cam = &Camera::get();
        if (cam->getAttachedEntity()->id == data.owner.id)
        {
            forward = cam->getForward();
            right = cam->getRight();

            //make movement paralllellLl to ground
            forward = glm::normalize(glm::cross({ 0, 1, 0 }, right));
            right = glm::normalize(glm::cross(forward, {0, 1, 0}));
        }

        else
        {
            forward = transform->getForward(data.owner);
            right = transform->getRight(data.owner);
        }

        bool wake = false;

        if (glfwGetKey(window, data.forward))
        {
            physics->getRigidBody(data.owner)->applyCentralForce(glmToBullet(data.movementSpeed * forward));
            wake = true;
        }

        else if (glfwGetKey(window, data.backward))
        {
            physics->getRigidBody(data.owner)->applyCentralForce(glmToBullet(data.movementSpeed * -forward));
            wake = true;
        }

        if (glfwGetKey(window, data.strafeRight))
        {
            physics->getRigidBody(data.owner)->applyCentralForce(glmToBullet(data.movementSpeed * right));
            wake = true;
        }

        else if (glfwGetKey(window, data.strafeLeft))
        {
            physics->getRigidBody(data.owner)->applyCentralForce(glmToBullet(data.movementSpeed * -right));
            wake = true;
        }

        if (wake)
            physics->getRigidBody(data.owner)->activate();
    }
}

void PlayerInputComp::printImguiDebug(Entity entity)
{
    using namespace ImGui;

    if (hasInput(entity))
    {
        Keys keys = getKeys(entity);
        std::string forward = glfwGetKeyName(keys.forward, glfwGetKeyScancode(keys.forward));
        std::string backward = glfwGetKeyName(keys.backward, glfwGetKeyScancode(keys.backward));
        std::string left = glfwGetKeyName(keys.strafeLeft, glfwGetKeyScancode(keys.strafeLeft));
        std::string right = glfwGetKeyName(keys.strafeRight, glfwGetKeyScancode(keys.strafeRight));
        Text("Forward: %s\nBackwards: %s\nLeft: %s\nRight: %s\n",
            forward.c_str(), backward.c_str(), left.c_str(), right.c_str());

        
        DragFloat(("Movement speed " + std::to_string(entity.id)).c_str(), &keys.movementSpeed, 0.1f);
        setKeys(entity, keys);
    }

    else
    {
        if (Button(("Add input " + std::to_string(entity.id)).c_str()))
            addInput(entity);
    }
}