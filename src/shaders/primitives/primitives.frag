#version 330 core

out vec4 frag_color;

uniform vec3 color;

void main()
{
    frag_color = vec4(color.xyz, 1.0f);
}