#version 330 core
layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec3 gAlbedo;

in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;

void main()
{    
    // 储存片段的位置矢量到第一个G缓冲纹理
    gPosition = FragPos;
    // 储存法线信息到G缓冲
    gNormal = normalize(Normal);
    // 储存漫反射颜色到G缓冲
    gAlbedo.rgb = vec3(0.95);
}