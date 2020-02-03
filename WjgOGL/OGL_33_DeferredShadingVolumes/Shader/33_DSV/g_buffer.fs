#version 330 core
layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gAlbedoSpec;

in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;

void main()
{    
    // 把每个片段的位置向量存到G缓冲
    gPosition = FragPos;
    // 把每个片段的发现数据存放到G缓冲
    gNormal = normalize(Normal);
    // 把每个片段的漫反射颜色存放到G缓冲
    gAlbedoSpec.rgb = texture(texture_diffuse1, TexCoords).rgb;
    // 把每个片段的高光值存放到G缓冲
    gAlbedoSpec.a = texture(texture_specular1, TexCoords).r;
}