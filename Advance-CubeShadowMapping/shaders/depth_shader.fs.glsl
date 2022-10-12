#version 330 core
in vec4 FragPos;

uniform vec3 light_pos;
uniform float far_plane;

void main() {
    // float light_dis = length(FragPos.xyz - light_pos);
    gl_FragDepth = 0.5f;
}