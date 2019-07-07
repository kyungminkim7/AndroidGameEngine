#version 320 es

precision mediump float;

in vec2 vTexCoord;

uniform sampler2D texture0;
uniform vec4 color;

out vec4 gl_FragColor;

void main() {
    gl_FragColor = vec4(1.0, 1.0, 1.0, texture(texture0, vTexCoord).a) * color;
}
