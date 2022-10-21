#version 330 core
out vec4 FragColor;
in vec2 TexCoords;

uniform sampler2D samp;
uniform bool horizontal;
uniform float weight[5] = float[] (0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216);

void main(){
    vec4 ret = texture(samp, TexCoords) * weight[0];
    vec2 tex_offset = 1.f/textureSize(samp,0);
    for(int i =  1 ; i <  5 ; ++i ){
        if(horizontal){
            ret += texture(samp,TexCoords+tex_offset.y*i)*weight[i];
            ret += texture(samp,TexCoords-tex_offset.y*i)*weight[i];
        }else{
            ret += texture(samp,TexCoords+tex_offset.x*i)*weight[i];
            ret += texture(samp,TexCoords-tex_offset.x*i)*weight[i];
        }
    }
    FragColor = vec4(ret.rgb,1.f);
}
