#version 330 core
in vec2 Texcoord;
uniform sampler2D samp;
out vec4 FragColor;
void main(){
    FragColor = texture(samp,Texcoord);
}