attribute vec2 aPos;
attribute vec2 aTexCoord;

varying vec2 vTexCoord;

uniform mat4 projection;
uniform mat4 model;

void main() {
   gl_Position = projection * model * vec4(aPos, 0.0, 1.0);
   vTexCoord = aTexCoord;
}