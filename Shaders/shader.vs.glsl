#version 330 core
layout(location = 0) in vec3 apos;
layout(location = 1) in vec3 acolor;

out vec3 target_color;

void main() {
    //gl_Position = vec4(apos,1.f); //ordinary
    gl_Position = vec4(apos.x,-apos.y,apos.z,1.f); //rerse
    target_color = acolor;
}