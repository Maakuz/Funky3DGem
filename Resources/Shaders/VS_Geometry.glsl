#version 410 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 norm;

out VS_OUT
{
    vec3 pos;
    vec3 normal;
} vs_out;

uniform mat4 VP;
uniform mat4 WORLD;
uniform mat4 WORLDINVTR;

void main()
{
    vec4 newPos = vec4(pos, 1);

    gl_Position = VP * WORLD * newPos;

    vec4 newNorm;
    newNorm.xyz = norm;
    newNorm.w = 1;
    vs_out.normal = normalize((WORLDINVTR * newNorm).xyz);
    vs_out.pos = (WORLD * newPos).xyz;
}