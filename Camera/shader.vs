#version 330 core
layout(location = 0) in vec3 apos;
layout(location = 1) in vec3 aColor;
layout(location = 2) in vec2 aTex;

out vec3 next_color;
out vec2 next_tex;
uniform mat4 trans;
uniform mat4 mvp;
void main(){
    gl_Position = mvp *trans* vec4(apos,1.f);
    next_color = aColor;
    next_tex = aTex;
}   