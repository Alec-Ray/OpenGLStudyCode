#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;

//片段着色器会更加基础，我们做的唯一一件事就是从生成的纹理中采样
void main()
{    
    //FragColor = texture(texture1, TexCoords);
	FragColor = texture(screenTexture, TexCoords);
}