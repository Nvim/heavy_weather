#version 330 core

layout (location = 0) in vec3 pos;

uniform mat4 iTransform;

void main()
{
  // gl_Position = vec4(pos.x+iRotation, pos.y, pos.z, 1.0);
  gl_Position = iTransform * vec4(pos, 1.0f);
}
