#version 450

layout(location = 0) in vec2 i_position;
layout(location = 1) in vec2 i_uv;

layout(location = 0) out vec2 o_uv;

void main()
{
    o_uv = i_uv;
    gl_Position = vec4(i_position, 0.0f, 1.0f);
}