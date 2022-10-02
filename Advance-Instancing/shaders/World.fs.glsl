#version 330 core
in vec3 normal;
in vec2 tex_coord;
out vec4 FragColor;
struct Material{
    sampler2D texture_diffuse1;
    sampler2D texture_diffuse2;
    sampler2D texture_diffuse3;
    sampler2D texture_diffuse4;
    sampler2D texture_diffuse5;

    sampler2D texture_specular1;
    sampler2D texture_specular2;
    sampler2D texture_specular3;
    sampler2D texture_specular4;
    sampler2D texture_specular5;
    float shininess;
};
uniform Material material;
void main() {
    FragColor = texture(material.texture_diffuse1,tex_coord);
    //FragColor = vec4(1.f,1.f,1.f,1.f);
}