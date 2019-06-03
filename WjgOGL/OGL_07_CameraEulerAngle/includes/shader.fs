#version 330 core
out vec4 FragColor;

in vec3 ourColor;	// �Ӷ�����ɫ���������������(������ͬ��������ͬ)
in vec2 TexCoord;   // �Ӷ�����ɫ����������������

uniform sampler2D texture01;	//����1
uniform sampler2D texture02;	//����2

void main()
{
    //FragColor = vec4(ourColor, 1.0f);

	//ʹ��GLSL�ڽ���texture�����������������ɫ
	//����һ��������������������ڶ��������Ƕ�Ӧ����������
	//FragColor = texture(ourTexture,TexCoord);

	//���������ɫ��������������Ľ��
	//���������ֵ��0.0�����᷵�ص�һ�����룻
	//�����1.0���᷵�صڶ�������ֵ
	//0.2�᷵��80%�ĵ�һ��������ɫ��20%�ĵڶ���������ɫ����������������Ļ��ɫ��
	FragColor = mix(texture(texture01,TexCoord),texture(texture02,TexCoord),0.4);
}

