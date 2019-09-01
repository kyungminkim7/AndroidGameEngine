#version 320 es
#extension GL_OES_EGL_image_external_essl3 : require

precision mediump float;

in vec2 vTextureCoordinate;
uniform samplerExternalOES backgroundTexture;

out vec4 gl_FragColor;

void main() {
    gl_FragColor = texture(backgroundTexture, vTextureCoordinate);
}