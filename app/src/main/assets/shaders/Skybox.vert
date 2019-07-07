#version 320 es

in vec3 aPosition;

uniform mat4 projection_view;

out vec3 vTextureCoordinate;

void main(void)
{
    vec4 position = projection_view * vec4(aPosition, 1.0);
    gl_Position = position.xyww;

    vTextureCoordinate = aPosition;
}