#version 330 core
layout(location = 0) in vec3 apos;
layout(location = 1) in vec3 anormal;
layout(location = 2) in vec2 atex_coord;
layout(location = 3) in mat4 instanceMatrix;

out vec3 normal;
out vec2 tex_coord;
uniform mat4 projection;
uniform mat4 view;

void main() {
    gl_Position = projection * view * instanceMatrix * vec4(apos,1.f);
    normal = anormal;
    tex_coord = atex_coord;
}