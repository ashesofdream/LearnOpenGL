#version 330 core
in vec3 FragCoord;
in vec2 TexCoords;
in vec3 Normal;
uniform sampler2D samp;
struct Light{
    vec3 Position;
    vec3 Color;
};
uniform Light lights[4];
layout(location = 0) out vec4 FragColor;
layout(location = 1) out vec4 HighlightColor;
void main(){
    vec3 origin_color = texture(samp,TexCoords).rgb;
    vec3 ret_color = vec3(0.f,0.f,0.f);
    for(int i = 0 ; i < 4 ; ++i){
       vec3 light_dir =normalize(lights[i].Position - FragCoord);
       float cos_val = max(dot(light_dir,Normal),0); 
       float light_len = length(lights[i].Position-FragCoord);
       ret_color += lights[i].Color * origin_color * cos_val /(light_len*light_len);
    }
    FragColor = vec4(ret_color,1.f);
    float brightness = dot(FragColor.rgb, vec3(0.2126, 0.7152, 0.0722));
    if(brightness > 1.0)
        HighlightColor = vec4(FragColor.rgb, 1.0);
}