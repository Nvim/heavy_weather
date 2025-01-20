#version 330 core

in vec4 color;
out vec4 FragColor;

void main()
{
  FragColor = color;
  // FragColor = vec4(0.5, 1.0, 0.2, 1.0);
}
