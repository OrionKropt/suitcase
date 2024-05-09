#version 330 core

in vec3 _color;
out vec4 color;

void main() {
    color = vec4(_color, 1.0);
}