#version 450 core

// layed-out this way to optimise packing
struct PointLight {
  vec3 position; 
  float constant;
  vec3 ambient;
  float linear;
  vec3 diffuse;
  float quadratic;
  vec3 specular;
  float pad;
};

struct DirLight {
  vec3 direction; 
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
};

#define MAX_POINT_LIGHTS 5
layout(std140, binding=1) uniform Lights {
  DirLight dirlight;
  PointLight pointlight[MAX_POINT_LIGHTS];
};

in vec2 TexUvs;
in vec3 Normal;
in vec3 FragPos;
in vec3 ViewPos;
out vec4 FragColor;

uniform sampler2D TexDiffuse;
uniform sampler2D TexSpecular;

vec3 ComputeDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 ComputePointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
  
void main()
{
  vec3 norm = normalize(Normal);
  vec3 viewDir = normalize(ViewPos - FragPos);
  vec3 result = vec3(0.0);

  result += ComputeDirLight(dirlight, norm, viewDir);
  for(uint i = 0; i < MAX_POINT_LIGHTS; ++i) {
    result += ComputePointLight(pointlight[i], norm, FragPos, viewDir);
  }
  FragColor = vec4(result, 1.0);
}

vec3 ComputeDirLight(DirLight light, vec3 normal, vec3 viewDir) {
  // ambient
  vec3 ambient = light.ambient * texture(TexDiffuse, TexUvs).rgb;

  // diffuse
  vec3 lightDir = normalize(-light.direction);  
  float diff_fact = max(dot(normal, lightDir), 0.0);
  vec3 diffuse = light.diffuse * diff_fact * texture(TexDiffuse, TexUvs).rgb;

  // specular
  vec3 reflectDir = reflect(-lightDir, normal);
  float spec_fact = pow(max(dot(viewDir, reflectDir), 0.0), 32);
  vec3 specular = light.specular * spec_fact * texture(TexSpecular, TexUvs).rgb;

  vec3 result =  diffuse + ambient + specular;
  return result;
}

vec3 ComputePointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
  // ambient
  vec3 ambient = light.ambient * texture(TexDiffuse, TexUvs).rgb;

  // diffuse
  vec3 lightDir = normalize(light.position - fragPos);  
  float diff_fact = max(dot(normal, lightDir), 0.0);
  vec3 diffuse = light.diffuse * diff_fact * texture(TexDiffuse, TexUvs).rgb;

  // specular
  vec3 reflectDir = reflect(-lightDir, normal);
  float spec_fact = pow(max(dot(viewDir, reflectDir), 0.0), 32);
  vec3 specular = light.specular * spec_fact * texture(TexSpecular, TexUvs).rgb;

  float distance = length(light.position - fragPos);
  float den = max((light.constant + light.linear * distance + light.quadratic * (distance * distance)), 0.01);
  float attenuation = 1.0 / den;    

  ambient  *= attenuation; 
  diffuse  *= attenuation;
  specular *= attenuation;   
  vec3 result =  diffuse + ambient + specular;
  return result;
}
