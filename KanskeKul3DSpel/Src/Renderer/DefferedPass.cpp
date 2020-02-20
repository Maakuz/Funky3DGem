#include "DefferedPass.h"
#include "Debug/ConsoleWindow.h"
#include "Component/ModelComp.h"
#include "Component/TransformComp.h"
#include "Camera.h"

constexpr unsigned int x = 1920;
constexpr unsigned int y = 1080;

DefferedPass::DefferedPass():
    m_deffered({ Shader(SHADER_PATH "VS_Geometry.glsl", ShaderType::Vertex), Shader(SHADER_PATH "PS_Geometry.glsl", ShaderType::Fragment) })
{
    glGenFramebuffers(1, &m_gBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, m_gBuffer);

    glGenTextures(1, &m_posBuffer);
    glBindTexture(GL_TEXTURE_2D, m_posBuffer);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, x, y, 0, GL_RGB, GL_FLOAT, 0);

    glGenTextures(1, &m_normalBuffer);
    glBindTexture(GL_TEXTURE_2D, m_normalBuffer);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, x, y, 0, GL_RGB, GL_FLOAT, 0);

    glGenTextures(1, &m_colorBuffer);
    glBindTexture(GL_TEXTURE_2D, m_colorBuffer);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, x, y, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_posBuffer, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, m_normalBuffer, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, m_colorBuffer, 0);

    unsigned int attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
    glDrawBuffers(3, attachments);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        printfCon("Error in generating deffered buffers");

    m_deffered.initializeUniformLocation("VP");
    m_deffered.initializeUniformLocation("WORLD");
    m_deffered.initializeUniformLocation("WORLDINVTR");
}

void DefferedPass::draw(std::vector<Entity>* modelQueue)
{
    m_deffered.use();


    glBindFramebuffer(GL_FRAMEBUFFER, m_gBuffer);

    unsigned int attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
    glDrawBuffers(3, attachments);

    glViewport(0, 0, x, y);
    glClearColor(0, 0, 0, 255);
    glClear(GL_COLOR_BUFFER_BIT);

    glUniformMatrix4fv(m_deffered.getUniformID("VP"), 1, GL_FALSE, &Camera::get().getVP()[0][0]);

    for (Entity& entity : *modelQueue)
    {
        glUniformMatrix4fv(m_deffered.getUniformID("WORLD"), 1, GL_FALSE, &TransformComp::get().getTransformMat(entity)[0][0]);
        glUniformMatrix4fv(m_deffered.getUniformID("WORLDINVTR"), 1, GL_FALSE, &TransformComp::get().getInverseTransposeMat(entity)[0][0]);

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
