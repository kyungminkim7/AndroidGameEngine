#version 320 es

in vec3 aPosition;
in vec2 aTextureCoordinate;
in float aOpacity;

out vec4 vPositionLightSpace;

layout (std140) uniform ProjectionViewUB {
    mat4 projection_view;
};

layout (std140) uniform LightSpaceUB {
    mat4 lightSpace;
};

uniform mat4 model;

void main() {
    vec4 worldPosition = model * vec4(aPosition, 1.0);

    gl_Position = projection_view * worldPosition;
    vPositionLightSpace = lightSpace * worldPosition;
}
