#version 330 core
layout(location = 0) in vec2 apoint;
layout(location = 1) in vec2 atexcoord;

out vec2 tex_coord;
void main(){
    gl_Position = vec4(apoint,0.f,1.f);
    tex_coord = atexcoord;
}