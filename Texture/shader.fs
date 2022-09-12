#version 330 core
out vec4 FragColor;
in vec3 next_color;
in vec2 next_tex;
uniform sampler2D tex;
uniform sampler2D tex2;

void main(){
    FragColor = mix(texture(tex,next_tex),texture(tex2,next_tex) ,0.2);
}