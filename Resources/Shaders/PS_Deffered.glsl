#version 410 core
#define MAX_LIGHTS 20

uniform int DIR_LIGHT_COUNT;

//Data formatted as
//arr[0] = direction
//arr[1] = color
uniform vec3 DIR_LIGHTS[MAX_LIGHTS * 2];

uniform sampler2D POS;
uniform sampler2D NORMAL;
uniform sampler2D COLOR;

uniform int SHADOW_EXISTS;
uniform sampler2D SHADOW;
uniform mat4 LIGHT_VP;

in vec2 uv;
out vec3 color;
void main()
{
    vec4 wPos = vec4(texture(POS, uv).xyz, 1);
    vec3 normal = texture(NORMAL, uv).xyz;
    vec3 materialColor = texture(COLOR, uv).rgb;

    vec3 diffuse = vec3(0, 0, 0);
    for (int i = 0; i < DIR_LIGHT_COUNT * 2; i++)
    {
        diffuse += dot(-DIR_LIGHTS[i], normal) * DIR_LIGHTS[i + 1] * materialColor;
    }

    //Make this for the light that casts shadows only would be cool
    if (SHADOW_EXISTS == 1)
    {
        vec4 posFromlight = LIGHT_VP * wPos;

        posFromlight /= posFromlight.w;

        posFromlight = (posFromlight * 0.5) + 0.5;

        float depth = texture(SHADOW, posFromlight.xy).r;

        if (depth < posFromlight.z - 0.001)
        {
            diffuse = vec3(0, 0, 0);
        }
    }

    color = diffuse;
}