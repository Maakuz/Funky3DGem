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
    gl_Position.xyz = pos;
    gl_Position.w = 1.0;

    gl_Position = VP * WORLD * gl_Position;

    vs_out.pos = gl_Position.xyz;

    vec4 newNorm;
    newNorm.xyz = norm;
    newNorm.w = 1;
    vs_out.normal = normalize((WORLDINVTR * newNorm).xyz);
}