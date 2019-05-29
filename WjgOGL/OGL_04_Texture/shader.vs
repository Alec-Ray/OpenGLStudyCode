#version 330 core
layout (location = 0) in vec3 aPos;			// λ�ñ���������λ��ֵΪ 0 
layout (location = 1) in vec3 aColor;		// ��ɫ����������λ��ֵΪ 1
layout (location = 2) in vec2 aTexCoord;	// �������������λ��ֵΪ 2

out vec3 ourColor;		// ΪƬ����ɫ��ָ��һ����ɫ���
out vec2 TexCoord;		// ���������괫�ݸ�Ƭ����ɫ��

void main()
{
    gl_Position = vec4(aPos, 1.0);
    ourColor = aColor;
	TexCoord = aTexCoord;
}