#include "MovementInputComp.h"
#include "DataTemplate.h"
#include "MovementComp.h"
#include "TransformComp.h"
#include "../Camera.h"

void MovementInputComp::addInput(Entity entity)
{
    if (!MovementComp::get().hasComponent(entity))
    {
        printfCon("Entity need transform to have movement", entity.id);
        return;
    }

    addData<Keys>(m_dataMap, m_data, entity, Keys(entity));
}

void MovementInputComp::removeInput(Entity entity)
{
    removeData<Keys>(m_dataMap, m_data, entity);
}

void MovementInputComp::setKeys(Entity entity, Keys keys)
{
    m_data[m_dataMap[entity.id]] = keys;
}

void MovementInputComp::handleInputs(GLFWwindow* window)
{
    MovementComp* movement = &MovementComp::get();
    TransformComp* transform = &TransformComp::get();
    for (Keys& keys : m_data)
    {
        glm::vec3 forward;
        glm::vec3 right;
        Camera* cam = &Camera::get();
        if (cam->getAttachedEntity()->id == keys.owner.id)
        {
            forward = cam->getForward();
            right = cam->getRight();
        }

        else
        {
            forward = transform->getForward(keys.owner);
            right = transform->getRight(keys.owner);
        }

        if (glfwGetKey(window, keys.forward))
            movement->addAcceleration(keys.owner, keys.movementSpeed * forward);

        else if (glfwGetKey(window, keys.backward))
            movement->addAcceleration(keys.owner, keys.movementSpeed * -forward);


        if (glfwGetKey(window, keys.strafeRight))
            movement->addAcceleration(keys.owner, keys.movementSpeed * right);

        else if (glfwGetKey(window, keys.strafeLeft))
            movement->addAcceleration(keys.owner, keys.movementSpeed * -right);

    }
}

void MovementInputComp::printImguiDebug(Entity entity)
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