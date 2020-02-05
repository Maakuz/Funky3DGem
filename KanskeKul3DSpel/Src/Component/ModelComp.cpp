#include <GL/glew.h>
#include "ModelComp.h"
#include "DataTemplate.h"
#include "TransformComp.h"
#include "Importer/Importer.h"

ModelComp::ModelComp()
{
    Importer::Model mod = Importer::loadModelFromFile("../Resources/Models/cube.mop");

    MeshBuffer mesh(1, mod.vertices.size());

    m_meshes.emplace(Meshes::Cube, mesh);
    glGenBuffers(1, &m_meshes[Meshes::Cube].vertexBufferID);
    glBindBuffer(GL_ARRAY_BUFFER, m_meshes[Meshes::Cube].vertexBufferID);
    glBufferData(GL_ARRAY_BUFFER, 12 * mod.vertices.size(), &mod.vertices[0], GL_STATIC_DRAW);

    glGenBuffers(1, &m_meshes[Meshes::Cube].normalBufferID);
    glBindBuffer(GL_ARRAY_BUFFER, m_meshes[Meshes::Cube].normalBufferID);
    glBufferData(GL_ARRAY_BUFFER, 12 * mod.normals.size(), &mod.normals[0], GL_STATIC_DRAW);
}

void ModelComp::addModel(Entity entity)
{
    if (!TransformComp::get().hasTransform(entity))
    {
        printfCon("Entity need transform to have a model", entity.id);
        return;
    }


    addData<Model>(m_dataMap, m_data, entity, Model(entity));
}

void ModelComp::removeModel(Entity entity)
{
    removeData<Model>(m_dataMap, m_data, entity);
}

ModelComp::MeshBuffer ModelComp::getBuffer(Entity entity) const
{
    if (!m_dataMap.count(entity.id))
    {
        printfCon("Entity %d has no model", entity.id);
        return MeshBuffer(NO_MESH, 0);
    }

    return m_meshes.at(m_data[m_dataMap.at(entity.id)].mesh);
}

Meshes ModelComp::getMesh(Entity entity) const
{
    return m_data[m_dataMap.at(entity.id)].mesh;
}

void ModelComp::setMesh(Entity entity, Meshes mesh)
{
    if (!TransformComp::get().hasTransform(entity))
    {
        printfCon("Entity need transform to have a model", entity.id);
        return;
    }

    m_data[m_dataMap[entity.id]].mesh = mesh;
}

void ModelComp::printImguiDebug(Entity entity)
{
    using namespace ImGui;
    if (hasModel(entity))
    {
        Text("Buffer ID: %d, vertices: %d", getBuffer(entity).vertexBufferID, getBuffer(entity).size);
        if (BeginCombo(("Mesh " + std::to_string(entity.id)).c_str(), std::to_string((int)getMesh(entity)).c_str()))
        {
            for (int j = 0; j < 1; j++)
            {
                if (Selectable(std::to_string(j).c_str()))
                    setMesh(entity, (Meshes)j);
            }

            EndCombo();
        }
    }

    else
    {
        if (Button(("Add model " + std::to_string(entity.id)).c_str()))
            addModel(entity);
    }
}

