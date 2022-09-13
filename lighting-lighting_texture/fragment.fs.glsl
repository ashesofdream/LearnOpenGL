#version 330 core
//out vec4 FragColor;
struct Material{
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};
struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform Light light;
uniform vec3 object_color;
uniform vec3 light_color;
uniform vec3 light_pos;
uniform vec3 view_pos;
uniform Material material;

in vec3 FragPos;
in vec3 Normal;
in vec2 tex1_coord;

void main(){
    vec3 ambient_light = vec3(texture(material.diffuse, tex1_coord))*light.ambient;

    vec3 light_dir = normalize(light_pos - FragPos);
    float diffuse_coeffcient = max(0,dot(normalize(Normal) ,light_dir));
    vec3 diffuse_light =  diffuse_coeffcient * light.diffuse * vec3(texture(material.diffuse,tex1_coord));


    vec3 view_dir = normalize(view_pos - FragPos);
    vec3 bisector_vector = normalize(view_dir + light_dir);
    float specular_coeeffcient =pow(max(0,dot(Normal,bisector_vector)),material.shininess);
    vec3 specular_light = vec3(texture(material.specular,tex1_coord) * specular_coeeffcient) * light.specular;

   //gl_FragColor = vec4(material.diffuse,1.0);
    gl_FragColor = vec4( (ambient_light +diffuse_light+specular_light) *object_color, 1.0);
    //gl_FragColor = texture(material.specular,tex1_coord);
}
//void main()
//{
//    // 环境光
//    vec3 ambient = light.ambient* material.ambient;
//
//    // 漫反射
//    vec3 norm = normalize(Normal);
//    vec3 lightDir = normalize(light_pos - FragPos);
//    float diff = max(dot(norm, lightDir), 0.0);
//    vec3 diffuse = light.diffuse * (diff * material.diffuse);
//
//    // 镜面光
//    vec3 viewDir = normalize(view_pos - FragPos);
//    vec3 reflectDir = reflect(-lightDir, norm);
//    float spec = pow(max(dot(viewDir, reflectDir), 0.0), );
//    vec3 specular = light.specular * (spec * material.specular);
//
//    vec3 result = diffuse+ambient+specular;
//    gl_FragColor= vec4(result,1);
//}