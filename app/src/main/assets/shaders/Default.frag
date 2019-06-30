precision mediump float;

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

varying vec3 vPosition;
varying vec3 vNormal;
varying vec2 vTexCoord;

uniform vec3 viewPosition;
uniform Material material;

uniform DirectionalLight directionalLight;

Lighting calculateBaseLight(vec3 lightDirection, Lighting lighting);
vec3 calculateDirectionalLight();

void main() {
	gl_FragColor = vec4(calculateDirectionalLight(), 1.0);
}

Lighting calculateBaseLight(vec3 lightDirection, Lighting lighting) {
    // Calculates Blinn-Phong lighting
    Lighting result;

    // Sets ambient color the same as the diffuse color
    vec3 materialDiffuse = texture2D(material.diffuseTexture0, vTexCoord).rgb;
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
            texture2D(material.specularTexture0, vTexCoord).rgb;

    return result;
}

vec3 calculateDirectionalLight() {
    Lighting result = calculateBaseLight(directionalLight.direction,
                                         directionalLight.lighting);
    return result.ambient + result.diffuse + result.specular;
}
