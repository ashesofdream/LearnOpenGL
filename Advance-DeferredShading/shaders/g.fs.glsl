#version 330 core
in vec3 frag_pos;
in vec3 normal;
in vec2 tex_coord;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;

layout(location = 0) out vec3 g_frag_pos;
layout(location = 1) out vec3 g_normal;
layout(location = 2) out vec4 g_color;

void main(){
    g_frag_pos = frag_pos;
    g_normal = normal;
    g_color.rgb = texture(texture_diffuse1,tex_coord).rgb;
    g_color.a = texture(texture_specular1,tex_coord).r;
}