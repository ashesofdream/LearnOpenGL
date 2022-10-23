#version 330 core
in vec2 tex_coord;

struct Light {
    vec3 Position;
    vec3 Color;
    
    float Linear;
    float Quadratic;
};


const int NR_LIGHTS = 32;
uniform Light lights[NR_LIGHTS];
uniform vec3 view_pos;
uniform sampler2D color_samp;
uniform sampler2D normal_samp;
uniform sampler2D frag_pos_samp;


out vec4 out_color;
void main(){
    // const float ambient = 0.f;
    // vec3 ret_color;
    // vec3 origin_color = texture(color_samp,tex_coord).rgb;
    // for(int i = 0 ; i < NR_LIGHTS ; ++i ){
    //     vec3 light_dir = lights[i].Position - texture(frag_pos_samp,tex_coord).rgb;
    //     vec3 n_light_dir = normalize(light_dir);
    //     float light_coefficient = dot( texture(normal_samp,tex_coord).rgb, n_light_dir);
    //     float light_dis = length(light_dir);
    //     ret_color += light_coefficient * lights[i].Color * origin_color /(light_dis*light_dis);
    // }
    // out_color = vec4(ret_color,1.f);
    out_color = vec4(1.f,1.f,0.f,1.f);
}