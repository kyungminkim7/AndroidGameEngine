#version 320 es

precision mediump float;
precision mediump sampler2DShadow;

struct Lighting {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct DirectionalLight {
    vec3 direction;
    Lighting lighting;
};

struct Material {
    float specularExponent;
    sampler2D diffuseTexture0;
    sampler2D specularTexture0;
};

in vec3 vPosition;
in vec3 vNormal;
in vec2 vTextureCoordinate;
in vec4 vPositionLightSpace;

uniform vec3 viewPosition;
uniform Material material;

uniform DirectionalLight directionalLight;
uniform sampler2D shadowMap;

const float minShadowBias = 0.0005;
const float maxShadowBias = 0.001;

out vec4 gl_FragColor;

Lighting calculateBaseLight(vec3 lightDirection, Lighting lighting);
float calculateShadow(vec3 lightDirection);

void main() {
    Lighting baseLighting = calculateBaseLight(directionalLight.direction,
                                               directionalLight.lighting);
	gl_FragColor = vec4(baseLighting.ambient + (1.0 - calculateShadow(directionalLight.direction))
                        * (baseLighting.diffuse + baseLighting.specular), 1.0);
}

Lighting calculateBaseLight(vec3 lightDirection, Lighting lighting) {
    // Calculates Blinn-Phong lighting
    Lighting result;

    // Sets ambient color the same as the diffuse color
    vec3 materialDiffuse = texture(material.diffuseTexture0, vTextureCoordinate).rgb;
    result.ambient = lighting.ambient * materialDiffuse;

    // Fragment is brighter the closer it is aligned to the light ray direction
    float lightAngle = max(dot(vNormal, -lightDirection),
                           0.0);
    result.diffuse = lighting.diffuse * lightAngle * materialDiffuse;

    // Specular light is brighter the closer the angle btwn the reflected
    // light ray and the viewing vector.
    vec3 viewDirection = normalize(viewPosition - vPosition);
    vec3 halfwayDirection = normalize(-lightDirection + viewDirection);
    float specularAngle = dot(halfwayDirection, vNormal);

    result.specular = lighting.specular *
            pow(max(specularAngle, 0.0), material.specularExponent) *
            texture(material.specularTexture0, vTextureCoordinate).rgb;

    return result;
}

float calculateShadow(vec3 lightDirection) {
    vec3 projectedCoordinates = vPositionLightSpace.xyz / vPositionLightSpace.w;
    projectedCoordinates = projectedCoordinates * 0.5 + 0.5;

    // Keep objects outside of the light's depth range in the light
    if (projectedCoordinates.z > 1.0) return 0.0;

    // Remove shadow acne
    float bias = max(maxShadowBias * (1.0 - dot(vNormal, -lightDirection)), minShadowBias);

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