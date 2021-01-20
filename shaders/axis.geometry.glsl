#version 410 core

layout (lines ) in;
layout (line_strip, max_vertices = 2) out;

uniform mat4 MVP;

out vec4 axisColour;


void createLine(vec4 vtxStart_, vec4 vtxEnd_)
{
    gl_Position = MVP * vtxStart_;
    EmitVertex();
    gl_Position = MVP * vtxEnd_;
    EmitVertex();
}

void main()
{
    vec4 vtx0 = gl_in[0].gl_Position;
    vec4 vtx1 = gl_in[1].gl_Position;

    vec4 dir = normalize(vtx1 - vtx0);
    axisColour = dir;

    createLine(vtx0,vtx1);

    EndPrimitive();
}
