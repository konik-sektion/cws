#version 330 core
in vec2 vUV;
out vec4 FragColor;

uniform sampler2D uScalar;
uniform sampler1D uLut;
uniform float uVmin;
uniform float uVmax;
uniform float uAlpha;

void main() {
    float v = texture(uScale, vUV).r;
    float t = (v - uVmin) / max(uVmax - uVmin, 1e-6);
    t = clamp(t, 0.0, 1.0);

    vec4 c = texture(uLut, t);
    c.a *= uAlpha;

    FragColor = c;
}
