#version 320 es

in vec3 aPosition;

uniform mat4 lightSpace;
uniform mat4 model;

void main() {
    gl_Position = lightSpace * model * vec4(aPosition, 1.0);
}
