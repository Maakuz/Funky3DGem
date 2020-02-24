#include "DirectionalShadow.h"
#include "glm/gtc/matrix_transform.hpp"

DirectionalShadow::DirectionalShadow() :
    m_map(2048, true)
{
    m_assignedEntity = nullptr;
    m_projection = glm::ortho(-10.f, 10.f, -10.f, 10.f, 0.1f, 100.f);
    m_view = glm::identity<glm::mat4>();
    m_vp = glm::identity<glm::mat4>();
    m_dirty = true;
}


void DirectionalShadow::calculateVP()
{ 
    if (m_dirty)
    {
        m_vp = m_projection * m_view;
        m_dirty = false;
    }
}

void DirectionalShadow::assignEntity(Entity* entity)
{
     m_assignedEntity = entity;
}

void DirectionalShadow::setView(glm::mat4 view)
{
    m_view = view;
    m_dirty = true;
}
