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

void Camera::calculateVP()
{
    if (m_attachedEntity)
        m_pos = TransformComp::get().getPosition(*m_attachedEntity);



    m_view = glm::lookAt(m_pos, m_dir, m_up);

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

void Camera::cameraDebug()
{
    using namespace ImGui;
    ImGui::Begin("Camera debug window!");

    if (BeginTabBar("tabs"))
    {
        if (BeginTabItem("Vectors"))
        {
            ImGui::Text("Pos");
            ImGui::Text("x: %f, y: %f, z %f", m_pos.x, m_pos.y, m_pos.z);

            ImGui::Text("Up");
            ImGui::Text("x: %f, y: %f, z %f", m_up.x, m_up.y, m_up.z);

            ImGui::Text("Dir");
            ImGui::Text("x: %f, y: %f, z %f", m_dir.x, m_dir.y, m_dir.z);
            EndTabItem();
        }

        if (BeginTabItem("Matrices"))
        {
            ImGui::Text("Pos");
            ImGui::Text("x: %f, y: %f, z %f", m_pos.x, m_pos.y, m_pos.z);

            ImGui::Text("Up");
            ImGui::Text("x: %f, y: %f, z %f", m_up.x, m_up.y, m_up.z);

            ImGui::Text("Dir");
            ImGui::Text("x: %f, y: %f, z %f", m_dir.x, m_dir.y, m_dir.z);
            EndTabItem();
        }

        ImGui::EndTabBar();
    }
    ImGui::End();
}
