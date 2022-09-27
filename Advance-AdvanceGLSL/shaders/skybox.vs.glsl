#version 420 core
layout(location = 0) in vec3 apos;

out vec3 TexCoords;

layout(std140,binding = 2) uniform Matrices{
    mat4 view;
    mat4 projection;
};

void main(){
    TexCoords = apos;
    gl_Position =(projection * mat4(mat3(view)) *   vec4(apos,1.f)).xyww;
    //  
}