#version 410 core

in vec3 vertColour;
out vec4 outColour;

uniform vec3 camPos;
void main ()
{
  // set the fragment colour to the current texture
  outColour = vec4(vertColour,1.0);
}
