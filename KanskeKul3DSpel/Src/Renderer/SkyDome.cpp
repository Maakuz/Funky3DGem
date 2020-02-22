#include "SkyDome.h"
#include "FullScreenQuad.h"
#include "Constants.h"
#include "Camera.h"

SkyDome::SkyDome() :
    m_shader({ Shader(SHADER_PATH "VS_Sky.glsl", ShaderType::Vertex), Shader(SHADER_PATH "PS_Sky.glsl", ShaderType::Fragment) })
{
    m_shader.initializeUniformLocation("VP");

    m_skyModel = ModelComp::get().getBuffer(1);
}

void SkyDome::renderSky(unsigned int geometryFBO)
{
    m_shader.use();
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    glFrontFace(GL_CW);
    
    glBindFramebuffer(GL_READ_FRAMEBUFFER, geometryFBO);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    glBlitFramebuffer(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
    
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    //remove position from view
    glm::mat4 view = glm::mat3(Camera::get().getView());
    glm::mat4 proj = Camera::get().getProjection();

    glm::mat4 vp = proj * view;
    glUniformMatrix4fv(m_shader.getUniformID("VP"), 1, GL_FALSE, &vp[0][0]);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, m_skyModel.vertexBufferID);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glDrawArrays(GL_TRIANGLES, 0, m_skyModel.size);
    glDisableVertexAttribArray(0);

    glDepthFunc(GL_LESS);
    glFrontFace(GL_CCW);
}
