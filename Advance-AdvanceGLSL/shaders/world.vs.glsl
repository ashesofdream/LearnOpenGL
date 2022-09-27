#version 420 core
layout(location=0) in vec3 aPos;
layout(location=1) in vec2 aTexcoord;

uniform mat4 model;
layout(std140,binding = 2) uniform Matrices{
    mat4 view;
    mat4 projection;
};
out vec2 Texcoord;

void main(){
    gl_Position = projection * view * model * vec4(aPos,1.f);
    Texcoord = aTexcoord;
}