#include "LightComp.h"
#include "DataTemplate.h"
#include "../Camera.h"

void DirectionalLightComp::addComponent(Entity entity)
{
    addData<DirectionalLight>(m_dataMap, m_lights, entity, DirectionalLight(entity));
    m_owner.push_back(entity);
}

void DirectionalLightComp::removeComponent(Entity entity)
{
    if (!m_dataMap.count(entity.id))
    {
        printfCon("Entity %d does not have this component", entity.id);
        return;
    }

    Entity last = m_owner.back();
    unsigned int index = m_dataMap[entity.id];

    unordered_erase(m_lights, m_lights.begin() + index);
    m_dataMap[last.id] = index;
    m_dataMap.erase(entity.id);

    m_owner[entity.id] = m_owner.back();
    m_owner.pop_back();
}

DirectionalLightComp::DirectionalLight DirectionalLightComp::getLight(Entity entity) const
{
    if (!m_dataMap.count(entity.id))
    {
        printfCon("Entity %d has no light", entity.id);
        return DirectionalLight(ENTITY_ERROR);
    }

    return m_lights[m_dataMap.at(entity.id)];
}

void DirectionalLightComp::setLight(Entity entity, DirectionalLight light)
{
    if (!m_dataMap.count(entity.id))
    {
        printfCon("Entity %d has no light", entity.id);
        return;
    }

    m_lights[m_dataMap.at(entity.id)] = light;
}

void DirectionalLightComp::setDir(Entity entity, glm::vec3 dir)
{
    if (!m_dataMap.count(entity.id))
    {
        printfCon("Entity %d has no light", entity.id);
        return;
    }

    m_lights[m_dataMap.at(entity.id)].dir = dir;
}

void DirectionalLightComp::setColor(Entity entity, glm::vec3 color)
{
    if (!m_dataMap.count(entity.id))
    {
        printfCon("Entity %d has no light", entity.id);
        return;
    }

    m_lights[m_dataMap.at(entity.id)].color = color;
}

void DirectionalLightComp::setAsShadowCaster(Entity entity)
{
    if (!m_dataMap.count(entity.id))
    {
        printfCon("Entity %d has no light", entity.id);
        return;
    }

    if (m_shadow.assignedEntity)
        delete m_shadow.assignedEntity;

    m_shadow.assignedEntity = new Entity(entity);
}

void DirectionalLightComp::calculateView()
{
    Camera* cam = &Camera::get();

    if (m_shadow.assignedEntity)
    {
        glm::vec3 pos = cam->getPos() + (-m_lights[m_dataMap[m_shadow.assignedEntity->id]].dir * 10.f);
        m_shadow.view = glm::lookAt(pos, cam->getPos(), { 0, 1, 0 });
    }
}

void DirectionalLightComp::printImguiDebug(Entity entity)
{
    using namespace ImGui;

    if (hasComponent(entity))
    {
        DragFloat3(("Direction " + std::to_string(entity.id)).c_str(), &m_lights[m_dataMap[entity.id]].dir.x, 0.01, -1, 1);
        DragFloat3(("Color " + std::to_string(entity.id)).c_str(), &m_lights[m_dataMap[entity.id]].color.x, 0.01, 0, 1);

        m_lights[m_dataMap[entity.id]].dir = glm::normalize(m_lights[m_dataMap[entity.id]].dir);

        if (Button(("Assign shadow caster " + std::to_string(entity.id)).c_str()))
            setAsShadowCaster(entity);
    }

    else
    {
        if (Button(("Add light " + std::to_string(entity.id)).c_str()))
            addComponent(entity);
    }

}



