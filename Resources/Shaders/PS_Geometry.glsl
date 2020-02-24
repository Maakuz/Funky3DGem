#version 410 core

in VS_OUT
{
    vec3 pos;
    vec3 normal;
} ps_in;

layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gColor;

uniform vec3 MATCOL;

void main()
{

    gPosition = ps_in.pos;
    gNormal = ps_in.normal;
    gColor = vec4(MATCOL.xyz, 1);
}