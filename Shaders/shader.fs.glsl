#version 330 core
out vec4 FragColor;
in vec3 target_color;

void main(){
	FragColor =  vec4(target_color,0);
}
