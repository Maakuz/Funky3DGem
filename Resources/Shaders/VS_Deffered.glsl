#version 410 core
#define MAX_LIGHTS 20

layout(location = 0) in vec3 pos;

out vec2 uv;
void main()
{
    vec4 newPos = vec4(pos, 1);

    gl_Position.xyz = pos;
    gl_Position.w = 1;

    uv = (pos.xy + 1) * 0.5;
}