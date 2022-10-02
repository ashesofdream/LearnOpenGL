#version 330 core
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
in VS_OUT{
    vec3 normal;
    vec3 frag_pos;
    vec2 tex_coord;
}fs_in;
uniform vec3 eye_pos;
struct FixLight{
    vec3 light_dir;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform FixLight light;

out vec4 FragColor;
void main() {
    vec3 view_dir = normalize(eye_pos - fs_in.frag_pos);
    vec3 text_diffuse = vec3(texture(material.texture_diffuse1,fs_in.tex_coord));

    vec3 ambient_color = text_diffuse ;

    float diffuse_cofficient = max(0,dot(fs_in.normal,light.light_dir));
    vec3 diffuse_color = diffuse_cofficient * light.diffuse * text_diffuse;

    vec3 bi_sector = normalize(view_dir+light.light_dir);
    float specular_coeeffcient = pow(max(0,dot(bi_sector,fs_in.normal)),32);
    vec3 specular_color = specular_coeeffcient*light.specular*vec3(texture(material.texture_specular1,fs_in.tex_coord));

    FragColor = vec4(diffuse_color+ambient_color+specular_color,1.f);
    //FragColor = vec4((text_diffuse*light.ambient),1.f);
}