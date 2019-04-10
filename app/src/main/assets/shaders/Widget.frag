precision mediump float;

varying vec2 vTexCoord;

uniform sampler2D texture0;
uniform vec4 color;

void main() {
    gl_FragColor = vec4(1.0, 1.0, 1.0, texture2D(texture0, vTexCoord).a) * color;
}
