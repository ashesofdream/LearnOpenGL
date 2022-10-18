#version 330 core
in vs_out {
    vec3 real_frag_pos;
    vec2 tex_coord;
    vec3 real_light_pos;
    vec3 real_view_pos;
    vec3 test_color;
} fs_in;

out vec4 FragColor;
uniform sampler2D normal_map;
uniform sampler2D texture_map;
uniform sampler2D parallax_map;
uniform float height_scale;

vec2 parallax_texcoord(vec2 origin_tex_coord,vec3 view_dir){
    return origin_tex_coord - view_dir.xy/view_dir.z * texture(parallax_map,origin_tex_coord).r *0.1;
}
vec2 steep_parallax_texcoord(vec2 origin_tex_coord,vec3 view_dir){
    float numLayers = 10;
    float layerDepth = 1.0/numLayers;
    float current_layer_depth = 0.f;
    vec2 offset_dir = view_dir.xy/view_dir.z*height_scale;
    float deltaTexCoords = 
    return offset_dir;
}
void main(){
    
    vec3 view_dir = normalize(fs_in.real_view_pos - fs_in.real_frag_pos);
    vec3 light_dir = normalize(fs_in.real_light_pos - fs_in.real_frag_pos);
    vec2 real_tex_coord = parallax_texcoord(fs_in.tex_coord,view_dir);
    if(real_tex_coord.x > 1.0 || real_tex_coord.y > 1.0 || real_tex_coord.x < 0.0 ||real_tex_coord.y < 0.0)discard;
    vec3 normal = texture(normal_map,real_tex_coord).xyz;
    normal = normalize(normal * 2.0 - 1.0);
    vec4 text = texture(texture_map,real_tex_coord);
    
    
    
    vec3 ambient = 0.1 * text.xyz;
    float diffuse_coefficient = max(0.f,dot(normal,light_dir));
    vec3 diffuse = diffuse_coefficient * text.xyz;
    vec3 bisector_dir = normalize(-light_dir+view_dir);
    float specular_coefficient = pow(max(0.f,dot(bisector_dir,normal)),32);
    vec3 specular = specular_coefficient * text.xyz;
 
    FragColor = vec4(ambient+diffuse+specular,1.f);
    //FragColor = vec4(fs_in.test_color,1.f);
}
