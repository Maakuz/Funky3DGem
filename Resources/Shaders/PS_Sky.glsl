#version 410 core

in VS_OUT
{
    vec3 pos;
    vec3 normal;
} ps_in;

out vec3 color;
void main()
{

    color = vec3(0, 1, 1);
}