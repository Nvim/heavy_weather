#version 450 core


in vec2 TexUvs;
in vec3 Normal;
in vec3 FragPos;
layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 FragColor2;
layout (location = 2) out vec4 FragColor3;

uniform sampler2D TexDiffuse;
uniform sampler2D TexSpecular;

void main()
{
  FragColor = texture(TexDiffuse, TexUvs);
  FragColor2 = vec4(FragPos, 1.0);
  FragColor3 = vec4(normalize(Normal), 1.0);
}

