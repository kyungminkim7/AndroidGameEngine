#version 320 es

in vec3 aPosition;
in vec3 aNormal;
in vec2 aTextureCoordinate;

out vec3 vPosition;
out vec3 vNormal;
out vec2 vTextureCoordinate;
out vec4 vPositionLightSpace;

layout (std140) uniform ProjectionViewUB {
   mat4 projection_view;
};

layout (std140) uniform LightSpaceUB {
   mat4 lightSpace;
};

uniform mat4 model;
uniform mat3 normal;

void main() {
   vec4 worldPosition = model * vec4(aPosition, 1.0);
   gl_Position = projection_view * worldPosition;
   vPosition = vec3(worldPosition);
   vNormal = normalize(vec3(normal * aNormal));
   vTextureCoordinate = aTextureCoordinate;
   vPositionLightSpace = lightSpace * worldPosition;
}