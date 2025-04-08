#version 330 core

in vec2 TexUvs;
in vec3 Normal;
in vec3 FragPos;
out vec4 FragColor;

uniform sampler2D TexDiffuse;
uniform vec3 lightPos;

// vec3 lightPos = vec3(5.0, 5.0, 1.0);
float AMBIENT_STRENGTH = 0.3;
vec3 AMBIENT_COLOR = vec3(1.0, 1.0, 1.0);

void main()
{
  vec3 ambient = AMBIENT_STRENGTH * AMBIENT_COLOR;

  vec3 norm = normalize(Normal);
  vec3 lightDir = normalize(lightPos - FragPos);  
  float diffuse_factor = max(dot(norm, lightDir), 0.0);

  FragColor = texture(TexDiffuse, TexUvs)*vec4(ambient+diffuse_factor, 1.0);

  // FragColor = vec4(1.0);
  // FragColor.x = abs(Normal.x);
  // FragColor.y = abs(Normal.y);
  // FragColor.z = abs(Normal.z);
}
