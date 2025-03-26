#version 330 core

out vec4 frag_color;

in vec2 px_position;

uniform vec3 color;
uniform vec2 center;
uniform float outer_radius;
uniform float inner_radius;

void main()
{
    float distance = length(px_position - center);

    if (distance > outer_radius || distance < inner_radius)
    {
        discard;
    }

    frag_color = vec4(color, 1.0f);
}
