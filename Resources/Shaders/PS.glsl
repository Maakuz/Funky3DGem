#version 410 core
#define MAX_LIGHTS 20

in VS_OUT
{
    vec3 pos;
    vec3 normal;
} ps_in;

struct DirLight
{
    vec4 dir;
    vec4 color;
};

uniform int DIR_LIGHT_COUNT;

//Data formatted as
//arr[0] = direction
//arr[1] = color
uniform vec3 DIR_LIGHTS[MAX_LIGHTS * 2];

out vec3 color;
void main()
{
    vec3 diffuse = vec3(0, 0, 0);
    for (int i = 0; i < DIR_LIGHT_COUNT * 2; i++)
    {
        diffuse += dot(-DIR_LIGHTS[i], ps_in.normal.xyz) * DIR_LIGHTS[i + 1];
    }


    color = diffuse;
}