#version 410 core
#define MAX_LIGHTS 20

uniform int DIR_LIGHT_COUNT;

//Data formatted as
//arr[0] = direction
//arr[1] = color
uniform vec3 DIR_LIGHTS[MAX_LIGHTS * 2];

//uniform sampler2D SHADOW_MAP;

uniform sampler2D POS;
uniform sampler2D NORMAL;
uniform sampler2D COLOR;

in vec2 uv;
out vec3 color;
void main()
{
    vec3 diffuse = vec3(0, 0, 0);
    for (int i = 0; i < DIR_LIGHT_COUNT * 2; i++)
    {
        diffuse += dot(-DIR_LIGHTS[i], texture(NORMAL, uv).xyz) * DIR_LIGHTS[i + 1] * texture(COLOR, uv).xyz;
    }


    color = diffuse;
}