#version 330 core

out vec4 FragColor;

uniform float iGlobalTime;
uniform vec4 iMaterial;
uniform int flag;

void main()
{
  float time = 1.0;
  if (flag == 1) {
    time = clamp(abs(sin(iGlobalTime)), 0.3, 1.0);
  }
  FragColor.r = iMaterial.r * time;
  FragColor.g = iMaterial.g * time;
  FragColor.b = iMaterial.b * time;
  FragColor.a = iMaterial.a;
}
