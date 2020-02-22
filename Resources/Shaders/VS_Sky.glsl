#version 410 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 norm;


out vec3 pos0;

uniform mat4 VP;

void main()
{
    vec4 newPos = vec4(pos, 1);

    gl_Position = VP * newPos;

    gl_Position = newPos.xyww;
}