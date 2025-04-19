#version 450 core
// TODO: compatibility for 330 (binding not supported)

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 uvs;

layout (std140, binding=0) uniform Matrices {
  mat4 projection;
  mat4 view;
  vec3 camera_world; // camera in world space
};
uniform mat4 Model;

void main()
{
  gl_Position = projection * view * Model * vec4(pos, 1.0f);
}
