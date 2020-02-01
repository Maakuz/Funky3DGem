#include <GL/glew.h>
#include "ModelComp.h"
#include "DataTemplate.h"
#include "TransformComp.h"
#include "Cube.h"

ModelComp::ModelComp()
{
    GLfloat vertexData[] =
    {
        -1, -1, 0,
         1, -1, 0,
         0,  1, 0
    };

    MeshBuffer mesh(0, 3);

    m_meshes.emplace(Meshes::Triangle, mesh);
    glGenBuffers(1, &m_meshes[Meshes::Triangle].bufferID);
    glBindBuffer(GL_ARRAY_BUFFER, m_meshes[Meshes::Triangle].bufferID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), &vertexData, GL_STATIC_DRAW);

    MeshBuffer mesh2(1, cubeVertices);

    m_meshes.emplace(Meshes::Cube, mesh2);
    glGenBuffers(1, &m_meshes[Meshes::Cube].bufferID);
    glBindBuffer(GL_ARRAY_BUFFER, m_meshes[Meshes::Cube].bufferID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube), &cube, GL_STATIC_DRAW);
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

