#version 410 core


in vec3 pos0;

out vec3 color;
void main()
{
    float lerp = (pos0.y * 0.5) + 0.5;
    color = mix( vec3(0, 0.5, 1), vec3(0, 0.8, 1), lerp);
    color.r = lerp;
}