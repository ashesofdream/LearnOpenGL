/*
    this shader is used to direct output a texture image.
    what you need to do just draw a rectangle.
*/
#version  330 core 
layout(location = 0) in vec2 apos;
layout(location = 1) in vec2 atex_coord;
out vec2 tex_coord;
void main(){
    gl_Position = vec4(apos.x,apos.y,0.f,1.f);
    tex_coord = atex_coord;
}