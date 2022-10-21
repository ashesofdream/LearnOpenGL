#version 330 core
in vec3 FragCoord;
in vec2 TexCoords;
in vec3 Normal;
uniform sampler2D samp;
layout(location = 0) out vec4 FragColor;
layout(location = 1) out vec4 HighlightColor;
void main(){
    FragColor = texture(samp,TexCoords);

    float brightness = dot(FragColor.rgb, vec3(0.2126, 0.7152, 0.0722));
    if(brightness > 1.0)
        HighlightColor = vec4(FragColor.rgb, 1.0);
}