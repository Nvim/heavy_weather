#version 330 core

out vec4 FragColor;

uniform float iGlobalTime;
uniform vec4 iMaterial;

void main()
{
  // float time = sin(2 * iGlobalTime);
  // vec4 col = color;
  // col.r += 0.5 * time;
  // col.g += 2 * time;
  // col.b += 0.1 * time;
  // col.a = 1.0;

  FragColor = iMaterial;
}
