#version 320 es

in vec3 aPos;

uniform mat4 projection_view;

out vec3 vTexCoord;

void main(void)
{
    vec4 pos = projection_view * vec4(aPos, 1.0);
    gl_Position = pos.xyww;

    vTexCoord = aPos;
}