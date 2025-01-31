#version 330 core

in vec4 color;
out vec4 FragColor;

uniform float iGlobalTime;

void main()
{
  float time = sin(2 * iGlobalTime);
  vec4 col = color;
  col.r += 0.5 * time;
  col.g += 2 * time;
  col.b += 0.1 * time;
  col.a = 1.0;

  FragColor = col;
  // FragColor = vec4(0.5, 1.0, 0.2, 1.0);
}
