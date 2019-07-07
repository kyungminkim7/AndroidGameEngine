#version 320 es

in vec3 aPosition;
in vec3 aNormal;
in vec2 aTextureCoordinate;

out vec3 vPosition;
out vec3 vNormal;
out vec2 vTextureCoordinate;
out vec4 vPositionLightSpace;

uniform mat4 projection_view;
uniform mat4 model;
uniform mat3 normal;
uniform mat4 lightSpace;

void main() {
   gl_Position = projection_view * model * vec4(aPosition, 1.0);
   vPosition = vec3(model * vec4(aPosition, 1.0));
   vNormal = normalize(vec3(normal * aNormal));
   vTextureCoordinate = aTextureCoordinate;
   vPositionLightSpace = lightSpace * vec4(vPosition, 1.0);
}