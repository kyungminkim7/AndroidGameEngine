#version 320 es

precision mediump float;

uniform vec3 color;

out vec4 gl_FragColor;

void main() {
    gl_FragColor = vec4(color, 1.0);
}
