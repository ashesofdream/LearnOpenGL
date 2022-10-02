#version 420 core
layout(location = 0) in vec3 apos;
layout(location = 1) in vec3 anormal;
layout(location = 2) in vec2 tex_coord;

layout(std140,binding = 0) uniform Matrices{
    mat4 projection;
    mat4 view;
};

uniform mat4 model;
out VS_OUT{
    vec3 normal;
    vec3 frag_pos;
    vec2 tex_coord;
} vs_out;

void main(){
    vs_out.frag_pos = vec3(model*vec4(apos,1.0f));
    gl_Position = projection*view*model*vec4(apos,1.0f);
    float a = length(vec3(projection[0]));
    float b = length(vec3(projection[1]));
    float c = length(vec3(projection[2]));
    vs_out.normal =vec3(vec4(anormal[0]/a,anormal[1]/b,anormal[2]/c,1.f) * model);
    vs_out.tex_coord = tex_coord;
}