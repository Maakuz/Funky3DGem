#version 410 core

layout(location = 0) in vec3 pos;
layout(location = 0) in vec3 test;

layout(location = 0) out vec3 outPos;

uniform mat4 VP;
uniform mat4 World;
void main()
{
    gl_Position.xyz = pos;
    gl_Position.w = 1.0;

    gl_Position = VP * World * gl_Position;

    outPos = gl_Position.xyz;
}