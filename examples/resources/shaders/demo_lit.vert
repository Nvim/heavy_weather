#version 330 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 uvs;

uniform mat4 MVP;
uniform mat4 Model;

out vec3 Normal;
out vec2 TexUvs;
out vec3 FragPos; // Fragment position in world space

void main()
{
  // gl_Position = vec4(pos.x+iRotation, pos.y, pos.z, 1.0);
  gl_Position = MVP * vec4(pos, 1.0f);
  FragPos = vec3(Model * vec4(pos, 1.0));
  TexUvs = uvs;
  Normal = normal;
}
