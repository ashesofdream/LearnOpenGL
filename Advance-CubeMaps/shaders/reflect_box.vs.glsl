#version 330 core
layout(location = 0) vec3 apos;
layout(location = 1) vec3 anormal;
layout(location = 2) vec2 aTexcoord;

out vec3 normal;
out vec3 FragPos;
out vec3 Texcoord;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

void main(){
    FragPos = model * apos;
    gl_Position = projection * view * FragPos;
    normal = normalize(vec3(vec4(anormal[0]/length(vec3(model[0])),
    anormal[1]/length(vec3(model[1])) ,
    anormal[2]/length(vec3(model[2])) ,
    0 )*model));;
    Texcoord = aTexcoord;
}