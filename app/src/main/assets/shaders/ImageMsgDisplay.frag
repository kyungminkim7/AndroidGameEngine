#version 320 es

precision mediump float;

in vec2 vTextureCoordinate;
uniform sampler2D cameraImageTexture;

out vec4 gl_FragColor;

void main() {
    gl_FragColor = texture(cameraImageTexture, vTextureCoordinate);
}
