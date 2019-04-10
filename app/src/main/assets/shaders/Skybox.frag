precision mediump float;

varying vec3 vTexCoord;

uniform samplerCube skybox;

void main(void) {
    gl_FragColor = textureCube(skybox, vTexCoord);
}