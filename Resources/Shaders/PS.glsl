#version 410 core

layout(location = 0) in vec3 pos;

out vec3 color;
void main()
{
    color = vec3(0, 1, 0.2);
}