#version 440 core

layout (location=0) in vec3 inVert;

uniform mat4 MVP;

void main()
{
  gl_Position = MVP*vec4(inVert,1.0);
}
