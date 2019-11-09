#version 330 core
layout (location = 0) in vec3 aPos;

//ͶӰ����͹۲��߾����������䣬���Էŵ�uniform���嵱��
layout (std140) uniform Matrices
{
	mat4 projection;
	mat4 view;
};

//ģ�;����ڲ�ͬ����ɫ���в�һ�������Ե����ó���
uniform mat4 model;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}  
