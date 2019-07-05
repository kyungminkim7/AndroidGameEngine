#version 320 es

in vec3 aPos;
in vec3 aNormal;
in vec2 aTexCoord;

out vec3 vPos;
out vec3 vNormal;
out vec2 vTexCoord;
out vec4 vPosLightSpace;

uniform mat4 projection_view;
uniform mat4 model;
uniform mat3 normal;
uniform mat4 lightSpaceMatrix;

void main() {
   gl_Position = projection_view * model * vec4(aPos, 1.0);
   vPos = vec3(model * vec4(aPos, 1.0));
   vNormal = normalize(vec3(normal * aNormal));
   vTexCoord = aTexCoord;
   vPosLightSpace = lightSpaceMatrix * vec4(vPos, 1.0);
}