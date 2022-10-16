#version 330 core
in vs_out {
    vec3 real_frag_pos;
    vec2 tex_coord;
    vec3 real_light_pos;
    vec3 real_view_pos;
} fs_in;

out vec4 FragColor;
uniform sampler2D normal_map;
uniform sampler2D texture_map;
void main(){
    vec3 normal = texture(normal_map,fs_in.tex_coord).xyz;
    normal = normalize(normal * 2.0 - 1.0);
    vec4 text = texture(texture_map,fs_in.tex_coord);
    
    vec3 view_dir = normalize(fs_in.real_view_pos - fs_in.real_frag_pos);
    vec3 light_dir = normalize(fs_in.real_light_pos - fs_in.real_frag_pos);
    vec3 ambient = 0.1 * text.xyz;
    float diffuse_coefficient = max(0.f,dot(normal,light_dir));
    vec3 diffuse = diffuse_coefficient * text.xyz;
    vec3 bisector_dir = normalize(-light_dir+view_dir);
    float specular_coefficient = pow(max(0.f,dot(bisector_dir,normal)),32);
    vec3 specular = specular_coefficient * text.xyz;

    FragColor = vec4(ambient+diffuse+specular,1.f);
}
