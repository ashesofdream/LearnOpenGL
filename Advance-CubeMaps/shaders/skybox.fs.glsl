#version 330 core
in vec3 TexCoords;

uniform sampleCube skybox;
out vec4 FragColor;
void main(){
    FragColor = texture(skybox,TexCoords);
}
