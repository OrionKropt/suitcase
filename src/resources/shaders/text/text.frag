#version 330 core

// layout (location = 1) vec2 texture;

in vec2 texture_coords;
out vec4 color;

uniform sampler2D text;
uniform vec3 text_color;

void main()
{
    vec4 sampled = vec4(1.0f, 1.0f, 1.0f, texture(text, texture_coords).r);
    color = vec4(text_color, 1.0f) * sampled;
}
