#version 330 core
layout(location = 0) in vec3 apos;
layout(location = 1) in vec3 anormal;
layout(location = 2) in vec2 atex_coord;
layout(location = 3) in vec3 atangent;
layout(location = 4) in vec3 abitangent;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 light_pos;
uniform vec3 view_pos;


out vs_out {
    vec3 real_frag_pos;
    vec2 tex_coord;
    vec3 real_light_pos;
    vec3 real_view_pos;
    vec3 test_color;
} fs_in;



void main(){
    gl_Position = projection * view * model * vec4(apos,1.f);
    vec3 frag_pos = vec3(model*vec4(apos,1.f));
    fs_in.tex_coord = atex_coord;
    
    mat3 normalMatrix = transpose(inverse(mat3(model)));
    vec3 T = normalize(normalMatrix * atangent);
    vec3 B = normalize(normalMatrix * abitangent);
    vec3 N = normalize(normalMatrix * anormal);    
    
    mat3 TBN = transpose(mat3(T, B, N));  
    fs_in.real_light_pos = TBN * light_pos;
    fs_in.real_view_pos  = TBN * view_pos;
    fs_in.real_frag_pos = TBN * frag_pos;
    
}