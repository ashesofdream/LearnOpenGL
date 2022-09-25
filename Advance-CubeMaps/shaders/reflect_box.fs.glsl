#version 330 core
uniform samplerCube samp;
uniform vec3 eye_pos;
in vec3 normal;
in vec3 FragPos;
out vec4 FragColor;
void main(){
    vec3 in_dir = normalize(FragPos - eye_pos);
    vec3 out_dir = reflect(in_dir,normalize(normal));
    FragColor = texture(samp,normalize(normal));
}