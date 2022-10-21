#version 330 core
layout(location = 0) in vec2 apoint;
layout(location = 1) in vec2 atexcoord;

out vec2 TexCoords;
void main(){
    gl_Position = vec4(apoint,1.f,1.f);
    TexCoords = atexcoord;
}