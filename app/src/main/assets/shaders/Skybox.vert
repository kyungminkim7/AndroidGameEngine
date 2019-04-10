attribute vec3 aPos;

uniform mat4 projection_view;

varying vec3 vTexCoord;

void main(void)
{
    vec4 pos = projection_view * vec4(aPos, 1.0);
    gl_Position = pos.xyww;

    vTexCoord = aPos;
}