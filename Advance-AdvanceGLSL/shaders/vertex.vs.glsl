#version 330 core
layout(location = 0) in vec3 apos;
layout(location = 1) in vec2 tex_coord;
out vec3 FragPos;
out vec3 Normal;
out vec2 Texcoord;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;


void main(){
    gl_Position = vec4(apos,1.f);
    FragPos = vec3(model * vec4(apos,1.f));
    Texcoord = tex_coord;
//    Normal = mat3(transpose(inverse(model))) * anormal;
    //     Normal = anormal;
}