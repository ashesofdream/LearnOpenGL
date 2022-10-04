#version  330 core
in  vec2 tex_coord;
uniform sampler2D samp;
out vec4 frag_color;
void main(){
    //frag_color = texture(samp,tex_coord);
    //-color
    // frag_color = vec4(sampvec3(1.-texture(samp,tex_coord)),1.0);
    vec4 pc = texture(samp,tex_coord);
    frag_color =vec4(vec3(pc.r),1.f);
    //float average =  0.2126 * frag_color.r + 0.7152 * frag_color.g + 0.0722 * frag_color.b;
    //frag_color = vec4(average,average,average,1.f);
}