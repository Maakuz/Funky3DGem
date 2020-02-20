#include "Renderer.h"
#include "Component/ModelComp.h"
#include "Component/TransformComp.h"
#include "Component/LightComp.h"
#include "Camera.h"
#include "glm/gtc/type_ptr.hpp"
#include "Debug/ConsoleWindow.h"

std::vector<Entity> Renderer::s_modelQueue;

constexpr float quad[] = 
{
     1, -1,  0,
    -1,  1,  0,
    -1, -1,  0,
             
     1,  1,  0,
    -1,  1,  0,
     1, -1,  0,
};

Renderer::Renderer() :
    m_forward({ Shader(SHADER_PATH "VS_Deffered.glsl", ShaderType::Vertex), Shader(SHADER_PATH "PS_Deffered.glsl", ShaderType::Fragment) }),
    m_shadowBuffer({ Shader(SHADER_PATH "VS_Shadow.glsl", ShaderType::Vertex), Shader(SHADER_PATH "PS_Shadow.glsl", ShaderType::Fragment) })
{
    m_forward.initializeUniformLocation("VP");
    m_forward.initializeUniformLocation("WORLD");
    m_forward.initializeUniformLocation("WORLDINVTR");

    m_forward.initializeUniformLocation("DIR_LIGHT_COUNT");
    m_forward.initializeUniformLocation("DIR_LIGHTS");

    m_forward.initializeUniformLocation("LIGHT_VP");
    m_forward.initializeUniformLocation("SHADOW_MAP");

    m_forward.initializeUniformLocation("NORMAL");
    m_forward.initializeUniformLocation("POS");
    m_forward.initializeUniformLocation("COLOR");

    m_shadowBuffer.initializeUniformLocation("VP");
    m_shadowBuffer.initializeUniformLocation("WORLD");

    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    glGenBuffers(1, &m_quadID);
    glBindBuffer(GL_ARRAY_BUFFER, m_quadID);
    glBufferData(GL_ARRAY_BUFFER, 12 * 6, &quad[0], GL_STATIC_DRAW);


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
    

    

    m_deffered.draw(&s_modelQueue);

    m_forward.use();



    //glUniform1i(m_forward.getUniformID("SHADOW_MAP"), shadow->map.getFrameBufferID());

    glUniform1iv(m_forward.getUniformID("DIR_LIGHT_COUNT"), 1, &size);

    if (size > 0)
        glUniform3fv(m_forward.getUniformID("DIR_LIGHTS"), size * 2, glm::value_ptr(lights->at(0).dir));

    


    ////Bind backbuffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, 1920, 1080);

    unsigned int pos = m_deffered.getPos();
    unsigned int normal = m_deffered.getNormal();
    unsigned int color = m_deffered.getColor();

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_deffered.getPos());
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, m_deffered.getNormal());
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, m_deffered.getColor());

    glUniform1i(m_forward.getUniformID("POS"), 0);
    glUniform1i(m_forward.getUniformID("NORMAL"), 1);
    glUniform1i(m_forward.getUniformID("COLOR"), 2);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, m_quadID);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glDrawArrays(GL_TRIANGLES, 0, 6);






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
            Image(id, { 200, 200 });
        }

        Image((void*)m_deffered.getPos(), { 200, 200 });
        SameLine();
        Image((void*)m_deffered.getNormal(), { 200, 200 });
        Image((void*)m_deffered.getColor(), { 200, 200 });


        End();
    }

}

void Renderer::queueModel(Entity entity)
{
    s_modelQueue.push_back(entity);
}
