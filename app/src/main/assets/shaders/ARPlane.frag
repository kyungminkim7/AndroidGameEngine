#version 320 es

precision mediump float;

in vec2 vTextureCoordinate;
in float vOpacity;

uniform sampler2D planeTexture;

out vec4 gl_FragColor;

float calculateShadow();

void main() {
    gl_FragColor = vec4(vec3(1.0), texture(planeTexture, vTextureCoordinate).r * vOpacity);
}