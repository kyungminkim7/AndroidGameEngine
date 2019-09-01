#version 320 es

precision mediump float;

in vec2 vTextureCoordinate;
in float vOpacity;

uniform sampler2D planeTexture;
uniform vec3 color;

out vec4 gl_FragColor;

void main() {
    gl_FragColor = vec4(color, texture(planeTexture, vTextureCoordinate).r * vOpacity);
}
