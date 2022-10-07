#version 330 core
layout(location = 0) in vec3 apos;
layout(location = 1) in vec3 anormal;
layout(location = 2) in vec2 tex_coord;
out vec3 FragPos;
out vec4 LightSpacePos;
out vec3 Normal;
out vec2 tex1_coord;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 LightSpaceTrans;


void main(){
    gl_Position = projection *view * model * vec4(apos,1.f);
    FragPos = vec3(model * vec4(apos,1.f));
    Normal = normalize(vec3(vec4(anormal[0]/length(vec3(model[0])),
    anormal[1]/length(vec3(model[1])) ,
    anormal[2]/length(vec3(model[2])) ,
    0 )*model));
    tex1_coord = tex_coord;
    LightSpacePos =LightSpaceTrans*vec4(FragPos,1.f);
//    Normal = mat3(transpose(inverse(model))) * anormal;
    //     Normal = anormal;
}