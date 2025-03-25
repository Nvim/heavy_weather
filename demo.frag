#version 330 core

in vec2 TexUvs;
out vec4 FragColor;

uniform float uBlendFactor;
uniform sampler2D uTexture;
uniform sampler2D uTexture2;

void main()
{
  FragColor = mix(texture(uTexture, TexUvs), texture(uTexture2, TexUvs), uBlendFactor);
}
