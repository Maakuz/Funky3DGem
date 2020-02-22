#include "Renderer.h"
#include "Component/ModelComp.h"
#include "Component/TransformComp.h"
#include "Component/LightComp.h"
#include "Camera.h"
#include "glm/gtc/type_ptr.hpp"
#include "Debug/ConsoleWindow.h"

std::vector<Entity> Renderer::s_modelQueue;



Renderer::Renderer() :
    m_shadowBuffer({ Shader(SHADER_PATH "VS_Shadow.glsl", ShaderType::Vertex), Shader(SHADER_PATH "PS_Shadow.glsl", ShaderType::Fragment) })
{
    m_shadowBuffer.initializeUniformLocation("VP");
    m_shadowBuffer.initializeUniformLocation("WORLD");

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
    const DirectionalLightComp::DirLightShadow* shadow = DirectionalLightComp::get().getShadow();
    int size = lights->size();
    
    //Shadow buffering
    m_shadowBuffer.use();

    if (shadow->assignedEntity)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, shadow->map.getFrameBufferID());
        glViewport(0, 0, shadow->map.getRes(), shadow->map.getRes());
        glClear(GL_DEPTH_BUFFER_BIT);

        glm::mat4 vp = shadow->vp();
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

    m_deffered.firstPass(&s_modelQueue);

    m_deffered.secondPass(&lights->at(0).dir, size);

    m_sky.renderSky(m_deffered.getFBO());

    s_modelQueue.clear();
}

void Renderer::rendererDebug()
{
    using namespace ImGui;
    if (m_showDebug)
    {
        Begin("Renderererererer debug", &m_showDebug);

        const DirectionalLightComp::DirLightShadow* shadow = DirectionalLightComp::get().getShadow();

        
        if (shadow->assignedEntity)
        {
            ImTextureID id = (void*)shadow->map.getFrameBufferID();
            Image(id, ImVec2(256, 144), ImVec2(0, 1), ImVec2(1, 0));
            SameLine();
        }

        Image((void*)m_deffered.getPos(), ImVec2(256, 144), ImVec2(0, 1), ImVec2(1, 0));
        Image((void*)m_deffered.getNormal(), ImVec2(256, 144), ImVec2(0, 1), ImVec2(1, 0));
        SameLine();
        Image((void*)m_deffered.getColor(), ImVec2(256, 144), ImVec2(0, 1), ImVec2(1, 0));
        Image((void*)m_deffered.getDepth(), ImVec2(256, 144), ImVec2(0, 1), ImVec2(1, 0));


        End();
    }

}

void Renderer::queueModel(Entity entity)
{
    s_modelQueue.push_back(entity);
}
