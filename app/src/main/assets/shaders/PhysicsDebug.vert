#version 320 es

in vec3 aPosition;

uniform mat4 projection_view;
uniform vec3 origin;
uniform vec3 basis;

void main() {
    gl_Position = projection_view * vec4(basis * aPosition + origin, 1.0);
}