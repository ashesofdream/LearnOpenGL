#version 330 core
layout(location = 0)in vec3 apos;
layout(location = 1)in vec3 anormal;
// layout(location = 2)in vec2 aTexcoord;

out vec3 normal;
out vec3 FragPos;
// out vec3 Texcoord;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

void main(){
    FragPos = vec3(model * vec4(apos,1.f));
    gl_Position = vec4(FragPos,1.f);
    normal = normalize(vec3(vec4(anormal[0]/length(vec3(model[0])),
    anormal[1]/length(vec3(model[1])) ,
    anormal[2]/length(vec3(model[2])) ,
    0 )*model));;
    // Texcoord = aTexcoord;
}