#version 320 es

precision mediump float;

in vec4 vPositionLightSpace;

uniform vec3 normal;

uniform sampler2D shadowMap;
uniform vec3 lightDirection;

const float minShadowBias = 0.0005;
const float maxShadowBias = 0.001;

out vec4 gl_FragColor;

float calculateShadow();

void main() {
    gl_FragColor = vec4(vec3(0.0), calculateShadow());
}

float calculateShadow() {
    vec3 projectedCoordinates = vPositionLightSpace.xyz / vPositionLightSpace.w;
    projectedCoordinates = projectedCoordinates * 0.5 + 0.5;

    // Keep objects outside of the light's depth range in the light
    if (projectedCoordinates.z > 1.0) return 0.0;

    // Remove shadow acne
    float bias = max(maxShadowBias * (1.0 - dot(normal, -lightDirection)), minShadowBias);

    float closestDepth = texture(shadowMap, projectedCoordinates.xy).r;
    float currentDepth = projectedCoordinates.z;

    // Sample surrounding texels to create softer shadows
    float shadow = 0.0;
    vec2 texelSize = 1.0 / vec2(textureSize(shadowMap, 0));
    for(int x = -1; x <= 1; ++x) {
        for(int y = -1; y <= 1; ++y) {
            float pcfDepth = texture(shadowMap, projectedCoordinates.xy + vec2(x, y) * texelSize).r;
            shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;
        }
    }
    shadow /= 9.0;

    return shadow;
}