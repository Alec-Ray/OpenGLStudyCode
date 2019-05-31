#version 330 core
layout (location = 0) in vec3 aPos;			// λ�ñ���������λ��ֵΪ 0 
layout (location = 1) in vec3 aColor;		// ��ɫ����������λ��ֵΪ 1
layout (location = 2) in vec2 aTexCoord;	// �������������λ��ֵΪ 2

out vec3 ourColor;		// ΪƬ����ɫ��ָ��һ����ɫ���
out vec2 TexCoord;		// ���������괫�ݸ�Ƭ����ɫ��

uniform mat4 transform;

void main()
{
    //gl_Position = vec4(aPos, 1.0f);
    gl_Position = transform * vec4(aPos, 1.0f);

    ourColor = aColor;

	TexCoord = aTexCoord;
	//TexCoord = vec2(aTexCoord , 1.0 - aTexCoord.y);
}