#version 330 core
uniform mat4 model;
layout(location = 0) in vec3 apos;
void main() {
    gl_Position =  model *vec4(apos,1.f);
}