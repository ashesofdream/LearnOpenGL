#version 330 core
layout(location = 0) in vec3 apos;

out vec3 TexCoords;

uniform mat4 projection;
uniform mat4 view;

void main(){
    TexCoords = apos;
    gl_Position =vec4(apos,1.f);
    // projection * view * 
}