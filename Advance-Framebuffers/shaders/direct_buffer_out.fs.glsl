#version  330 core
in  vec2 tex_coord;
uniform sampler2D samp;
out vec4 frag_color;
void main(){
    frag_color = texture(samp,tex_coord);
}