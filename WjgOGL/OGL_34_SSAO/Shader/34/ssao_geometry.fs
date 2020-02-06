#version 330 core
layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec3 gAlbedo;

in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;

void main()
{    
    // ����Ƭ�ε�λ��ʸ������һ��G��������
    gPosition = FragPos;
    // ���淨����Ϣ��G����
    gNormal = normalize(Normal);
    // ������������ɫ��G����
    gAlbedo.rgb = vec3(0.95);
}