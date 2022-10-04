#version 330 core
uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
layout(location = 0) in vec3 apos;
void main() {
    gl_Position = projection*view* model *vec4(apos,1.f);
}