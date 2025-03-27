#version 330 core

// in vec2 TexUvs;
out vec4 FragColor;

uniform float uGlobalTime;
uniform vec4 uMaterial;
uniform int uFlag;
// uniform sampler2D uTexture;

void main()
{
  float time = 1.0;
  if (uFlag == 1) {
    time = clamp(abs(sin(uGlobalTime)), 0.3, 1.0);
  }
  vec4 col;
  col.r = uMaterial.r * time;
  col.g = uMaterial.g * time;
  col.b = uMaterial.b * time;
  col.a = uMaterial.a;

  // FragColor = mix(texture(uTexture), col, 0.6);
  FragColor = col;
}
