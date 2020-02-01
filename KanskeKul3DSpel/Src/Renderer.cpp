#include "Renderer.h"
#include "Component/ModelComp.h"
#include "Component/TransformComp.h"

std::vector<Entity> Renderer::s_modelQueue;

Renderer::Renderer():
    m_vertex(SHADER_PATH "VS.glsl", ShaderType::Vertex),
    m_fragment(SHADER_PATH "PS.glsl", ShaderType::Fragment),
    m_program({m_vertex, m_fragment})
{
    m_program.initializeUniformLocation("VP");
    m_program.initializeUniformLocation("World");

    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);
}

void Renderer::draw(const Camera* camera)
{
    m_program.use();

    glUniformMatrix4fv(m_program.getUniformID("VP"), 1, GL_FALSE, &camera->getVP()[0][0]);
    glEnableVertexAttribArray(0);

    for (size_t i = 0; i < s_modelQueue.size(); i++)
    {
        glUniformMatrix4fv(m_program.getUniformID("World"), 1, GL_FALSE, &TransformComp::get().getTransformMat(s_modelQueue[i])[0][0]);
        
        ModelComp::MeshBuffer mesh = ModelComp::get().getBuffer(s_modelQueue[i]);

        glBindBuffer(GL_ARRAY_BUFFER, mesh.bufferID);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
        glDrawArrays(GL_TRIANGLES, 0, mesh.size);
    }

    glDisableVertexAttribArray(0);
    s_modelQueue.clear();
}

void Renderer::queueModel(Entity entity)
{
    s_modelQueue.push_back(entity);
}
