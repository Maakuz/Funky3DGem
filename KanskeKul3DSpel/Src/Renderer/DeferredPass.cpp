#include "DeferredPass.h"
#include "Debug/ConsoleWindow.h"
#include "Component/ModelComp.h"
#include "Component/TransformComp.h"
#include "Camera.h"
#include "FullScreenQuad.h"
#include "glm/gtc/type_ptr.hpp"
#include "Constants.h"


DeferredPass::DeferredPass():
    m_second({ Shader(SHADER_PATH "VS_Deffered.glsl", ShaderType::Vertex), Shader(SHADER_PATH "PS_Deffered.glsl", ShaderType::Fragment) }),
    m_first({ Shader(SHADER_PATH "VS_Geometry.glsl", ShaderType::Vertex), Shader(SHADER_PATH "PS_Geometry.glsl", ShaderType::Fragment) })
{
    glGenFramebuffers(1, &m_gBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, m_gBuffer);

    glGenTextures(1, &m_posBuffer);
    glBindTexture(GL_TEXTURE_2D, m_posBuffer);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, SCREEN_WIDTH, SCREEN_HEIGHT, 0, GL_RGB, GL_FLOAT, 0);

    glGenTextures(1, &m_normalBuffer);
    glBindTexture(GL_TEXTURE_2D, m_normalBuffer);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, SCREEN_WIDTH, SCREEN_HEIGHT, 0, GL_RGB, GL_FLOAT, 0);

    glGenTextures(1, &m_colorBuffer);
    glBindTexture(GL_TEXTURE_2D, m_colorBuffer);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, SCREEN_WIDTH, SCREEN_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

    // depth
    glGenTextures(1, &m_depthBuffer);
    glBindTexture(GL_TEXTURE_2D, m_depthBuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, SCREEN_WIDTH, SCREEN_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_posBuffer, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, m_normalBuffer, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, m_colorBuffer, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depthBuffer, 0);

    unsigned int attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
    glDrawBuffers(3, attachments);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        printfCon("Error in generating deffered buffers");

    m_first.initializeUniformLocation("VP");
    m_first.initializeUniformLocation("WORLD");
    m_first.initializeUniformLocation("WORLDINVTR");


    m_second.initializeUniformLocation("DIR_LIGHT_COUNT");
    m_second.initializeUniformLocation("DIR_LIGHTS");

    m_second.initializeUniformLocation("LIGHT_VP");
    m_second.initializeUniformLocation("SHADOW_MAP");

    m_second.initializeUniformLocation("NORMAL");
    m_second.initializeUniformLocation("POS");
    m_second.initializeUniformLocation("COLOR");
}

void DeferredPass::firstPass(std::vector<Entity>* modelQueue)
{
    m_first.use();


    glBindFramebuffer(GL_FRAMEBUFFER, m_gBuffer);

    unsigned int attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
    glDrawBuffers(3, attachments);

    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    glClearColor(0, 0, 0, 255);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUniformMatrix4fv(m_first.getUniformID("VP"), 1, GL_FALSE, &Camera::get().getVP()[0][0]);

    for (Entity& entity : *modelQueue)
    {
        glUniformMatrix4fv(m_first.getUniformID("WORLD"), 1, GL_FALSE, &TransformComp::get().getTransformMat(entity)[0][0]);
        glUniformMatrix4fv(m_first.getUniformID("WORLDINVTR"), 1, GL_FALSE, &TransformComp::get().getInverseTransposeMat(entity)[0][0]);

        ModelComp::ModelBuffer mesh = ModelComp::get().getBuffer(entity);

        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, mesh.vertexBufferID);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, mesh.normalBufferID);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);


        glDrawArrays(GL_TRIANGLES, 0, mesh.size);
    }

    glDisableVertexAttribArray(0);
}

void DeferredPass::secondPass(const glm::vec3* firstDirLight, int lightCount)
{
    m_second.use();
    glDisable(GL_DEPTH_TEST);


    ////Bind backbuffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, 1920, 1080);

    //Clear
    glClearColor(0, 0, 0, 255);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //glUniform1i(m_forward.getUniformID("SHADOW_MAP"), shadow->map.getFrameBufferID());
    
    glUniform1iv(m_second.getUniformID("DIR_LIGHT_COUNT"), 1, &lightCount);

    if (lightCount > 0)
        glUniform3fv(m_second.getUniformID("DIR_LIGHTS"), lightCount * 2, glm::value_ptr(*firstDirLight));


    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_posBuffer);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, m_normalBuffer);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, m_colorBuffer);

    glUniform1i(m_second.getUniformID("POS"), 0);
    glUniform1i(m_second.getUniformID("NORMAL"), 1);
    glUniform1i(m_second.getUniformID("COLOR"), 2);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, Quad::get().getID());
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glDrawArrays(GL_TRIANGLES, 0, 6);
    glDisableVertexAttribArray(0);
}
