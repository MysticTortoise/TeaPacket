#version 450

layout(location = 0) in vec2 i_position;
layout(location = 1) in vec4 i_color;

layout(location = 0) out vec4 o_color;

void main()
{
    o_color = i_color;
    gl_Position = vec4(i_position, 0.5f, 1.0f);
}