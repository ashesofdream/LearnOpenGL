#version 330 core
layout(triangles) in;
layout(line_strip,max_vertices=6)out;
layout(std140,binding=0) uniform Matrices{
    mat4 projection;
    mat4 view;
};
in VS_OUT{
    vec3 normal;
    vec3 frag_pos;
    vec2 tex_coord;
} gs_in[];

void GenerateLine(int index){
    gl_Position =  gl_in[index].gl_Position;
    EmitVertex();
    gl_Position = (gl_in[index].gl_Position + vec4(gs_in[index].normal,0.f)*0.4);
    EmitVertex();
    EndPrimitive();
}

void main() {
    GenerateLine(0);
    GenerateLine(1);
    GenerateLine(2);
}