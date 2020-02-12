#version 410 core

layout(location = 0) in vec3 pos;

uniform mat4 VP;
uniform mat4 WORLD;

void main()
{
    gl_Position.xyz = pos;
    gl_Position.w = 1.0;

    gl_Position = VP * WORLD * gl_Position;
}