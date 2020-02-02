#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include "Component/TransformComp.h"
#include "ConsoleWindow.h"
#include "GLFW/glfw3.h"

Camera::Camera(float fov, float screenWidth, float screenHeight)
{
    m_attachedEntity = nullptr;
    m_height = screenHeight;
    m_width = screenWidth;
    m_mouseSpeed = 4.f;

    m_pos = { 0, 0, 1 };
    m_forward = { 0, 0, -1 };
    m_up = { 0, 1, 0 };
    m_right = {1, 0, 0};

    m_view = glm::lookAt(m_pos, m_forward, m_up);
    m_projection = glm::perspective(glm::radians(90.f), screenWidth / screenHeight, 0.1f, 100.f);

    m_vp = m_projection * m_view;
}

void Camera::trackMouse(float deltaTime, float mouseX, float mouseY)
{
    static float yaw = 0;
    static float pitch = 0;
    yaw += m_mouseSpeed * deltaTime * mouseX;
    pitch += m_mouseSpeed * deltaTime * mouseY;

    if (pitch < -89)
        pitch = -89;

    else if (pitch > 89)
        pitch = 89;

    m_forward.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    m_forward.y = sin(glm::radians(pitch));
    m_forward.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    glm::normalize(m_forward);

    m_right = glm::cross(m_forward, m_up);

    //m_up = glm::cross(m_right, m_forward);
}

void Camera::calculateVP()
{
    if (m_attachedEntity)
        m_pos = TransformComp::get().getPosition(*m_attachedEntity);



    m_view = glm::lookAt(m_pos, m_pos + m_forward, m_up);

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

void Camera::cameraDebug(bool* isOpen)
{
    using namespace ImGui;
    ImGui::Begin("Camera debug window!", isOpen);

    if (BeginTabBar("tabs"))
    {
        if (BeginTabItem("Vectors"))
        {
            ImGui::Text("Pos");
            ImGui::Text("x: %f, y: %f, z %f", m_pos.x, m_pos.y, m_pos.z);

            ImGui::Text("Up");
            ImGui::Text("x: %f, y: %f, z %f", m_up.x, m_up.y, m_up.z);

            ImGui::Text("Forward");
            ImGui::Text("x: %f, y: %f, z %f", m_forward.x, m_forward.y, m_forward.z);

            ImGui::Text("Right");
            ImGui::Text("x: %f, y: %f, z %f", m_right.x, m_right.y, m_right.z);
            EndTabItem();
        }

        if (BeginTabItem("Matrices"))
        {
            ImGui::Text("Pos");
            ImGui::Text("x: %f, y: %f, z %f", m_pos.x, m_pos.y, m_pos.z);

            ImGui::Text("Up");
            ImGui::Text("x: %f, y: %f, z %f", m_up.x, m_up.y, m_up.z);

            ImGui::Text("Dir");
            ImGui::Text("x: %f, y: %f, z %f", m_forward.x, m_forward.y, m_forward.z);
            EndTabItem();
        }

        if (BeginTabItem("Settings"))
        {
            ImGui::DragFloat("Sensitivity", &m_mouseSpeed);


            EndTabItem();
        }

        ImGui::EndTabBar();
    }
    ImGui::End();
}
