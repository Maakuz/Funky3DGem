#version 410 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 norm;

layout(location = 0) out vec3 outPos;
layout(location = 1) out vec3 outNorm;

uniform mat4 VP;
uniform mat4 World;
void main()
{
    gl_Position.xyz = pos;
    gl_Position.w = 1.0;

    gl_Position = VP * World * gl_Position;

    outPos = gl_Position.xyz;

    vec4 newNorm;
    newNorm.xyz = norm;
    newNorm.w = 1;
    outNorm = (World * newNorm).xyz;
}