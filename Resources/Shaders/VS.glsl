#version 410 core

layout(location = 0) in vec3 pos;

layout(location = 0) out vec3 outPos;

uniform mat4 VP;
void main()
{
    gl_Position.xyz = pos;
    gl_Position.w = 1.0;

    gl_Position = gl_Position * VP;

    outPos = gl_Position.xyz;
}