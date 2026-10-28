#version 450 core

in vec3 vColor;
in vec2 vUv;

uniform sampler2D uTexture;
uniform bool uTextured;

out vec4 fragColor;

void main()
{
    // TODO A4.7b: sample texture when uTextured, otherwise keep the colored houses.
    // Hint: Sample with GLSL texture(uTexture, vUv). Select behavior using uTextured and multiply the sampled color by vColor.
    fragColor = vec4(vColor, 1.0);
}
