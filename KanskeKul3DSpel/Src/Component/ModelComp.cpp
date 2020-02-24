#include <GL/glew.h>
#include <filesystem>
#include "ModelComp.h"
#include "DataTemplate.h"
#include "Importer/Importer.h"

constexpr const char* MODEL_NAMES[2] = { "Cube", "Sphere" };

ModelComp::ModelComp()
{
    namespace fs = std::filesystem;


    fs::directory_iterator dir("../Resources/Models/");

    for (auto file : dir)
    {
        if (file.path().extension() == ".mop")
        {
            Importer::Model mod = Importer::loadModelFromFile(file.path().string());

            ModelBuffer model(0, mod.vertices.size());

            glGenBuffers(1, &model.vertexBufferID);
            glBindBuffer(GL_ARRAY_BUFFER, model.vertexBufferID);
            glBufferData(GL_ARRAY_BUFFER, 12 * mod.vertices.size(), &mod.vertices[0], GL_STATIC_DRAW);

            glGenBuffers(1, &model.normalBufferID);
            glBindBuffer(GL_ARRAY_BUFFER, model.normalBufferID);
            glBufferData(GL_ARRAY_BUFFER, 12 * mod.normals.size(), &mod.normals[0], GL_STATIC_DRAW);
            
            m_meshes.push_back(model);
        }
    }

   
}

void ModelComp::addComponent(Entity entity)
{
    if (!TransformComp::get().hasComponent(entity))
    {
        printfCon("Entity need transform to have a model", entity.id);
        return;
    }


    addData<Model>(m_dataMap, m_data, entity, Model(entity));
}

void ModelComp::removeComponent(Entity entity)
{
    removeData<Model>(m_dataMap, m_data, entity);
}

ModelComp::ModelBuffer ModelComp::getBuffer(Entity entity) const
{
    if (!m_dataMap.count(entity.id))
    {
        printfCon("Entity %d has no model", entity.id);
        return ModelBuffer(NO_MESH, 0);
    }

    return m_meshes.at(m_data[m_dataMap.at(entity.id)].mesh);
}

ModelComp::ModelBuffer ModelComp::getBuffer(unsigned int meshID) const
{
    if (meshID >= m_meshes.size())
    {
        printfCon("ID OOB");
        return ModelBuffer(NO_MESH, 0);
    }

    return m_meshes[meshID];
}

unsigned int ModelComp::getMesh(Entity entity) const
{
    if (!TransformComp::get().hasComponent(entity))
    {
        printfCon("Entity need transform to have a model", entity.id);
        return 0;
    }

    return m_data[m_dataMap.at(entity.id)].mesh;
}

void ModelComp::setColor(Entity entity, glm::vec3 color)
{
    if (!TransformComp::get().hasComponent(entity))
    {
        printfCon("Entity need transform to have a model", entity.id);
        return;
    }

    m_data[m_dataMap[entity.id]].color = color;
}

glm::vec3 ModelComp::getColor(Entity entity) const
{
    if (!TransformComp::get().hasComponent(entity))
    {
        printfCon("Entity need transform to have a model", entity.id);
        return glm::vec3(0, 0, 0);
    }

    return m_data[m_dataMap.at(entity.id)].color;
}

void ModelComp::setMesh(Entity entity, unsigned int mesh)
{
    if (!TransformComp::get().hasComponent(entity))
    {
        printfCon("Entity need transform to have a model", entity.id);
        return;
    }

    m_data[m_dataMap[entity.id]].mesh = mesh;
}

void ModelComp::printImguiDebug(Entity entity)
{
    using namespace ImGui;
    if (hasComponent(entity))
    {
        Text("Buffer ID: %d, vertices: %d", getBuffer(entity).vertexBufferID, getBuffer(entity).size);
        if (BeginCombo(("Mesh " + std::to_string(entity.id)).c_str(), std::to_string(getMesh(entity)).c_str()))
        {
            for (unsigned int j = 0; j < m_meshes.size(); j++)
            {
                if (Selectable(std::to_string(j).c_str()))
                    setMesh(entity, j);
            }

            EndCombo();
        }

        DragFloat3(("Color " + std::to_string(entity.id)).c_str(), &m_data[m_dataMap[entity.id]].color.x, 0.01, 0, 1);
    }

    else
    {
        if (Button(("Add model " + std::to_string(entity.id)).c_str()))
            addComponent(entity);
    }
}

