#version 330 core

out vec4 FragColor;

uniform vec4 iMaterial;

void main()
{
  FragColor = iMaterial;
}
