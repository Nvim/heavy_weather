#version 450 core

struct LightSource {
  vec3 position;
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
};

layout(std140, binding=1) uniform Lights {
  LightSource light;
};

in vec2 TexUvs;
in vec3 Normal;
in vec3 FragPos;
in vec3 ViewPos;
out vec4 FragColor;

uniform sampler2D TexDiffuse;
uniform sampler2D TexSpecular;
// uniform vec3 lightPos;

void main()
{
  // ambient
  vec3 ambient = light.ambient * texture(TexDiffuse, TexUvs).rgb;

  // diffuse
  vec3 norm = normalize(Normal);
  vec3 lightDir = normalize(light.position - FragPos);  
  float diff_fact = max(dot(norm, lightDir), 0.0);
  vec3 diffuse = light.diffuse * diff_fact * texture(TexDiffuse, TexUvs).rgb;

  // specular
  vec3 viewDir = normalize(ViewPos - FragPos);
  vec3 reflectDir = reflect(-lightDir, norm);
  float spec_fact = pow(max(dot(viewDir, reflectDir), 0.0), 32);
  vec3 specular = light.specular * spec_fact * texture(TexSpecular, TexUvs).rgb;

  vec3 result =  diffuse + ambient + specular;
  FragColor = vec4(result, 1.0);
}
