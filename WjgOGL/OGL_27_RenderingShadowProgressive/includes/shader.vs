#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;

out vec2 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    TexCoords = aTexCoords;    
    gl_Position = projection * view * model * vec4(aPos, 1.0);
 
	//����ͨ��OpenGL��glPointSize������������Ⱦ�����ĵ�Ĵ�С��
	//������Ҳ�����ڶ�����ɫ�����޸����ֵ
	//����Ĵ�С����Ϊ�ü��ռ�λ�õ�zֵ��Ҳ���Ƕ����۲��ߵľ��롣
	//��Ĵ�С�����Ź۲��߾ඥ������Զ������
    gl_PointSize = gl_Position.z;  
}