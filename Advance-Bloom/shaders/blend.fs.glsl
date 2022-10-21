#version 330 core
out vec4 FragColor;
in vec2 TexCoords;

uniform sampler2D origin;
uniform sampler2D gaussian;
uniform float exposure;

void main(){
    const float gamma = 2.2;
    vec4 origin_color = texture(origin,TexCoords);
    vec4 gaussian_color = texture(gaussian,TexCoords);
    vec3 hdr_color = (origin_color + gaussian_color).rgb;

    //tone mapping
    hdr_color = vec3(1.0f)-exp(-hdr_color*exposure);
    //gamma correct 
    hdr_color = pow(hdr_color,vec3(1.f/gamma));
    FragColor = vec4(hdr_color,1.f);
    FragColor = gaussian_color;
}