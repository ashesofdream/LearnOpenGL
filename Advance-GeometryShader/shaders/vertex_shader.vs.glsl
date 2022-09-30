#version  330 core
layout(location = 0) in vec3 apos;

void main(){
    gl_Position = vec4(apos,1.f);
}