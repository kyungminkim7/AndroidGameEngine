attribute vec3 aPos;
attribute vec3 aNormal;
attribute vec2 aTexCoord;

varying vec3 vPosition;
varying vec3 vNormal;
varying vec2 vTexCoord;

uniform mat4 projection_view;
uniform mat4 model;
uniform mat3 normal;

void main() {
   gl_Position = projection_view * model * vec4(aPos, 1.0);
   vPosition = vec3(model * vec4(aPos, 1.0));
   vNormal = normalize(vec3(normal * aNormal));
   vTexCoord = aTexCoord;
}