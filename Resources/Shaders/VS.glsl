#version 410 core

layout(location = 0) in vec3 pos;

layout(location = 0) out vec3 outPos;

void main()
{
    gl_Position.xyz = pos;
    gl_Position.w = 1.0;
    outPos = pos;
}