#version 320 es

precision mediump float;

in vec3 vTexCoord;

uniform samplerCube skybox;

out vec4 gl_FragColor;

void main(void) {
    gl_FragColor = texture(skybox, vTexCoord);
}