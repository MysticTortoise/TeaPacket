#version 450

layout(location = 0) in vec2 i_uv;

layout(location = 0) out vec4 o_color;

layout(binding = 0) uniform sampler2D u_mainTex;

void main()
{
    o_color = texture(u_mainTex, i_uv);
    if(o_color.a <= 0.1){
        discard;
    }
}