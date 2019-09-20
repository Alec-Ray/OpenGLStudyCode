#version 330 core
layout (location = 0) in vec3 aPos;

out vec3 TexCoords;

uniform mat4 projection;
uniform mat4 view;

void main()
{
	//������ɫ���к�����˼�Ĳ����ǣ����ǽ������λ��������Ϊ�����Ƭ����ɫ������������
	//͸�ӳ������ڶ�����ɫ������֮��ִ�еģ���gl_Position��xyz�������w������
	//�����ִ���Ȳ���С����֪������������z�������ڶ�������ֵ��
	//ʹ����Щ��Ϣ�����ǿ��Խ����λ�õ�z������������w��������z������Զ����1.0��
	//�����ӵĻ�����͸�ӳ���ִ��֮��z�������Ϊw / w = 1.0��
	TexCoords = aPos;
    vec4 pos = projection * view * vec4(aPos, 1.0);
    gl_Position = pos.xyww;
}