#version 320 es

in vec2 aPosition;
in vec2 aTextureCoordinate;

out vec2 vTextureCoordinate;

void main() {
    gl_Position = vec4(aPosition, 0.0, 1.0);
    vTextureCoordinate = aTextureCoordinate;
}
