#version 320 es

in vec2 aPosition;
in vec2 aTextureCoordinate;

out vec2 vTextureCoordinate;

uniform mat4 projection;
uniform mat4 model;

void main() {
   gl_Position = projection * model * vec4(aPosition, 0.0, 1.0);
   vTextureCoordinate = aTextureCoordinate;
}