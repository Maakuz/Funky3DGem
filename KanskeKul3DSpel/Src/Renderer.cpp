#include "Renderer.h"
#include "Component/ModelComp.h"
#include "Component/TransformComp.h"
#include "Component/LightComp.h"
#include "Camera.h"
#include "glm/gtc/type_ptr.hpp"
#include "Debug/ConsoleWindow.h"

std::vector<Entity> Renderer::s_modelQueue;

Renderer::Renderer() :
    m_forward({ Shader(SHADER_PATH "VS.glsl", ShaderType::Vertex), Shader(SHADER_PATH "PS.glsl", ShaderType::Fragment) }),
    m_shadowBuffer({ Shader(SHADER_PATH "VS_Shadow.glsl", ShaderType::Vertex), Shader(SHADER_PATH "PS_Shadow.glsl", ShaderType::Fragment) })
{
    m_forward.initializeUniformLocation("VP");
    m_forward.initializeUniformLocation("WORLD");
    m_forward.initializeUniformLocation("WORLDINVTR");
    m_forward.initializeUniformLocation("DIR_LIGHT_COUNT");
    m_forward.initializeUniformLocation("DIR_LIGHTS");

    m_shadowBuffer.initializeUniformLocation("VP");
    m_shadowBuffer.initializeUniformLocation("WORLD");

    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    m_showDebug = false;

    ConsoleWindow::get().addCommand("debugRenderer", [&](Arguments args)->std::string
        {
            m_showDebug = !m_showDebug;

            return "toggler";
        });
}

void Renderer::draw()
{
    const std::vector<DirectionalLightComp::DirectionalLight>* lights = DirectionalLightComp::get().getDirectionalLights();
    const std::vector<DirectionalLightComp::DirLightShadow>* shadows = DirectionalLightComp::get().getShadows();
    int size = lights->size();

    //Shadow buffering
    m_shadowBuffer.use();

    for (const DirectionalLightComp::DirLightShadow& shadow : *shadows)
    {
        if (shadow.castShadow)
        {
            glBindFramebuffer(GL_FRAMEBUFFER, shadow.map.getFrameBufferID());
            glViewport(0, 0, shadow.map.getRes(), shadow.map.getRes());
            glClear(GL_DEPTH_BUFFER_BIT);

            glm::mat4 vp = shadow.vp();
            glUniformMatrix4fv(m_shadowBuffer.getUniformID("VP"), 1, GL_FALSE, &vp[0][0]);

            for (size_t i = 0; i < s_modelQueue.size(); i++)
            {
                glUniformMatrix4fv(m_shadowBuffer.getUniformID("WORLD"), 1, GL_FALSE, &TransformComp::get().getTransformMat(s_modelQueue[i])[0][0]);

                ModelComp::ModelBuffer mesh = ModelComp::get().getBuffer(s_modelQueue[i]);

                glEnableVertexAttribArray(0);
                glBindBuffer(GL_ARRAY_BUFFER, mesh.vertexBufferID);
                glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

                glEnableVertexAttribArray(1);
                glBindBuffer(GL_ARRAY_BUFFER, mesh.normalBufferID);
                glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

                glDrawArrays(GL_TRIANGLES, 0, mesh.size);
            }
        }
    }



    //Forward rendering
    m_forward.use();




    glUniformMatrix4fv(m_forward.getUniformID("VP"), 1, GL_FALSE, &Camera::get().getVP()[0][0]);
    glUniform1iv(m_forward.getUniformID("DIR_LIGHT_COUNT"), 1, &size);

    if (size > 0)
        glUniform3fv(m_forward.getUniformID("DIR_LIGHTS"), size * 2, glm::value_ptr(lights->at(0).dir));

    //Bind backbuffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, 1920, 1080);

    for (size_t i = 0; i < s_modelQueue.size(); i++)
    {
        glUniformMatrix4fv(m_forward.getUniformID("WORLD"), 1, GL_FALSE, &TransformComp::get().getTransformMat(s_modelQueue[i])[0][0]);
        glUniformMatrix4fv(m_forward.getUniformID("WORLDINVTR"), 1, GL_FALSE, &TransformComp::get().getInverseTransposeMat(s_modelQueue[i])[0][0]);
        
        ModelComp::ModelBuffer mesh = ModelComp::get().getBuffer(s_modelQueue[i]);

        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, mesh.vertexBufferID);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, mesh.normalBufferID);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

        glDrawArrays(GL_TRIANGLES, 0, mesh.size);
    }
    
    glDisableVertexAttribArray(0);
    s_modelQueue.clear();
}

void Renderer::rendererDebug()
{
    using namespace ImGui;
    if (m_showDebug)
    {
        Begin("Renderererererer debug", &m_showDebug);

        const std::vector<DirectionalLightComp::DirLightShadow>* shadows = DirectionalLightComp::get().getShadows();

        for (const DirectionalLightComp::DirLightShadow& shadow : *shadows)
        {
            if (shadow.castShadow)
            {
                ImTextureID id = (void*)shadow.map.getFrameBufferID();
                float size = shadow.map.getRes();
                Image(id, { 200, 200});
            }
        }

        End();
    }

}

void Renderer::queueModel(Entity entity)
{
    s_modelQueue.push_back(entity);
}
