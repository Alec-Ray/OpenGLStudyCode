#version 330 core
out vec4 FragColor;

in vec3 ourColor;	// �Ӷ�����ɫ���������������(������ͬ��������ͬ)
in vec2 TexCoord;   // �Ӷ�����ɫ����������������

uniform sampler2D ourTexture;	//����һ��uniform sampler2D��һ��������ӵ�Ƭ����ɫ����

void main()
{
    //FragColor = vec4(ourColor, 1.0f);

	//ʹ��GLSL�ڽ���texture�����������������ɫ
	//����һ��������������������ڶ��������Ƕ�Ӧ����������
	FragColor = texture(ourTexture,TexCoord);
}

