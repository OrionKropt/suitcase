#version 330 core

layout (location = 0) in vec2 position;

out vec2 px_position;

void main()
{
    px_position = position;
    gl_Position = vec4(position.xy, 0.0f, 1.0f);
}
