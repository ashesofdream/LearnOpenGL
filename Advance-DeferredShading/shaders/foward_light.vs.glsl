#version 330 core
layout(location = 0) in vec3 apos;
layout(location = 1) in vec3 anormal;
layout(location = 2) in vec2 atexcoord;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 normal_matrix;
out vec3 frag_pos;
out vec3 normal;
out vec2 tex_coord;

void main(){
    frag_pos = vec3(model*vec4(apos,1.f));
    gl_Position = projection * view * vec4(frag_pos,1.f);
    tex_coord = atexcoord;
    normal = normalize(vec3(normal_matrix * vec4(anormal,1.f)));
}