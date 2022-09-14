#version 330 core
//out vec4 FragColor;
struct Material{
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

struct spot_light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    //for light attenuation
    float constant;
    float linear;
    float quadratic;
    //for spot light
    vec3 spot_dir;
    float cos_inner_cut_off;
    float cos_outer_cut_off;
};
struct dir_light{
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
struct point_light{
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float constant;
    float linear;
    float quadratic;
};
uniform spot_light spotLight;
uniform vec3 light_color;
uniform vec3 view_pos;
uniform Material material;
#define NR_POINT_LIGHTS 4
uniform point_light pointLights[NR_POINT_LIGHTS];

in vec3 FragPos;
in vec3 Normal;
in vec2 tex1_coord;

out vec4 FragColor;
vec3 CalcSpotLight(spot_light light );
vec3 CalcDirLight(dir_light light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(point_light light);
void main(){
    vec3 result = CalcSpotLight(spotLight);
    for(int i = 0 ; i < NR_POINT_LIGHTS ; i++){
        result += CalcPointLight(pointLights[i]);
    }
    FragColor = vec4(result,1.f);
}
vec3 CalcSpotLight(spot_light light ){
    //return light.position;
    vec3 light_dir = normalize(light.position - FragPos);

    vec3 ambient_light = vec3(texture(material.diffuse, tex1_coord))*light.ambient;
    float cos_light_spot = dot(-light.spot_dir,light_dir);
    float episilon = light.cos_inner_cut_off - light.cos_outer_cut_off;
    float spot_rate = clamp((cos_light_spot - light.cos_outer_cut_off)/episilon,0.f,1.f);

    float dis = length(light.position-FragPos);
    float attenuation = 1.0/(light.constant + light.linear*dis + light.quadratic * (dis * dis));

    float diffuse_coeffcient = max(0,dot(normalize(Normal) ,light_dir));
    vec3 diffuse_light =  diffuse_coeffcient * light.diffuse * vec3(texture(material.diffuse,tex1_coord));

    vec3 view_dir = normalize(view_pos - FragPos);
    vec3 bisector_vector = normalize(view_dir + light_dir);
    float specular_coeeffcient =pow(max(0,dot(Normal,bisector_vector)),material.shininess);
    vec3 specular_light = vec3(texture(material.specular,tex1_coord) * specular_coeeffcient) * light.specular;
    //gl_FragColor = vec4(material.diffuse,1.0);
    return (diffuse_light*attenuation +specular_light*attenuation)*spot_rate+ambient_light ;
}
vec3 CalcPointLight(point_light light){
    vec3 light_dir = normalize(light.position - FragPos);

    vec3 ambient_light = vec3(texture(material.diffuse, tex1_coord))*light.ambient;

    float dis = length(light.position-FragPos);
    float attenuation = 1.0/(light.constant + light.linear*dis + light.quadratic * (dis * dis));

    float diffuse_coeffcient = max(0,dot(normalize(Normal) ,light_dir));
    vec3 diffuse_light =  diffuse_coeffcient * light.diffuse * vec3(texture(material.diffuse,tex1_coord));

    vec3 view_dir = normalize(view_pos - FragPos);
    vec3 bisector_vector = normalize(view_dir + light_dir);
    float specular_coeeffcient =pow(max(0,dot(Normal,bisector_vector)),material.shininess);
    vec3 specular_light = vec3(texture(material.specular,tex1_coord) * specular_coeeffcient) * light.specular;
    //gl_FragColor = vec4(material.diffuse,1.0);
    return (diffuse_light*attenuation +specular_light*attenuation)+ambient_light ;
}


vec3 CalcDirLight(dir_light light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    // 漫反射着色
    float diff = max(dot(normal, lightDir), 0.0);
    // 镜面光着色
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // 合并结果
    vec3 ambient  = light.ambient  * vec3(texture(material.diffuse, tex1_coord));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.diffuse, tex1_coord));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, tex1_coord));
    return (ambient + diffuse + specular);
}
