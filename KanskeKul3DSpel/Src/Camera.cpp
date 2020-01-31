#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include "Component/TransformComp.h"
#include "ConsoleWindow.h"

Camera::Camera(float fov, float screenWidth, float screenHeight)
{
    m_attachedEntity = nullptr;

    m_pos = { 0, 0, 1 };
    m_dir = {0, 0, -1};
    m_up = {0, 1, 0};

    m_view = glm::lookAt(m_pos, m_dir, m_up);
    m_projection = glm::perspective(glm::radians(90.f), screenWidth / screenHeight, 0.1f, 100.f);

    m_vp = m_projection * m_view;
}

void Camera::attachCamera(Entity entity)
{
    if (TransformComp::get().hasTransform(entity))
    {
        delete m_attachedEntity;
        m_attachedEntity = new Entity(entity);
    }

    else
        printfCon("Entity %d has no tranform", entity.id);
}
