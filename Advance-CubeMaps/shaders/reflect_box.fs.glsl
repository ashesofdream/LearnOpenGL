#version 330 core
uniform samplerCube samp;
uniform vec3 eye_pos;
in vec3 normal;
in vec3 FragPos;
out vec4 FragColor;
void main(){
    // vec3 in_dir = normalize(FragPos - eye_pos);
    // vec3 out_dir = reflect(in_dir,normalize(normal));
    // FragColor = vec4(texture(samp,out_dir).rgb,1.0);

    float ratio = 1.f/1.52;
    vec3 I = normalize(FragPos - eye_pos);
    vec3 R = refract(I,normalize(normal) ,ratio );
    FragColor = vec4(texture(samp,R).rgb ,1.f );
}