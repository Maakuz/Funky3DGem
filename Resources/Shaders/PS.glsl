#version 410 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 normal;

out vec3 color;
void main()
{
    vec3 light = vec3(0, 1, 0);

    float diffuse = dot(light, normal.xyz);

    color = vec3(0, 1, 0.2) * diffuse;
}