#version 320 es

in vec3 aPosition;

layout (std140) uniform LightSpaceUB {
    mat4 lightSpace;
};

uniform mat4 model;

void main() {
    gl_Position = lightSpace * model * vec4(aPosition, 1.0);
}
