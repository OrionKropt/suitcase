#version 330 core

layout (lines) in;
layout (triangle_strip, max_vertices = 4) out;

uniform float line_width;

void main()
{
    vec2 p0 = gl_in[0].gl_Position.xy;
    vec2 p1 = gl_in[1].gl_Position.xy;

    vec2 dir = normalize(p1 - p0);

    vec2 ort = vec2(-dir.y, dir.x) * line_width / 2.0;

    vec2 p0L = p0 - ort;
    vec2 p0R = p0 + ort;
    vec2 p1L = p1 - ort;
    vec2 p1R = p1 + ort;

    gl_Position = vec4(p0L, 0.0, 1.0);
    EmitVertex();
    gl_Position = vec4(p0R, 0.0, 1.0);
    EmitVertex();
    gl_Position = vec4(p1L, 0.0, 1.0);
    EmitVertex();
    gl_Position = vec4(p1R, 0.0, 1.0);
    EmitVertex();

    EndPrimitive();
}
