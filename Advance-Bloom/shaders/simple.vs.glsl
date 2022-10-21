#version 330 core
layout(location = 0 ) in vec3 apoint;
layout(location = 1) in vec2 atexcoord;
layout(location = 2) in vec3 anormal;

uniform mat4 normal_matrix;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
out vec3 FragCoord;
out vec2 TexCoords;
out vec3 Normal;
void main(){
    FragCoord = vec3(model*vec4(apoint,1.f));
    gl_Position = projection*view*model*vec4(apoint,1.f);
    FragCoord = vec3(apoint);
    Normal = vec3(normal_matrix * vec4(anormal,1.f));
    //gl_Position = vec4(apoint,1.f);
}