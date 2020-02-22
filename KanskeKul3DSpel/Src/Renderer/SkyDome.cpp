#include "SkyDome.h"
#include "FullScreenQuad.h"
#include "Constants.h"

SkyDome::SkyDome() :
    m_shader({ Shader(SHADER_PATH "VS_Sky.glsl", ShaderType::Vertex), Shader(SHADER_PATH "PS_Sky.glsl", ShaderType::Fragment) })
{
    m_skyModel = ModelComp::get().getBuffer(0);
    //Depthmap from geometry pass neccesarenty
}

void SkyDome::renderSky(unsigned int geometryFBO)
{
    m_shader.use();
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    glBindFramebuffer(GL_READ_FRAMEBUFFER, geometryFBO);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    glBlitFramebuffer(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
    
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, Quad::get().getID());
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glDrawArrays(GL_TRIANGLES, 0, 6);
    glDisableVertexAttribArray(0);

    glDepthFunc(GL_LESS);
}
