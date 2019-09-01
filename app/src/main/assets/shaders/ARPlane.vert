#version 320 es

in vec3 aPosition;
in vec2 aTextureCoordinate;
in float aOpacity;

out vec2 vTextureCoordinate;
out float vOpacity;

uniform mat4 projection_view;
uniform mat4 model;

void main() {
    gl_Position = projection_view * model * vec4(aPosition, 1.0);
    vTextureCoordinate = aTextureCoordinate;
    vOpacity = aOpacity;
}
