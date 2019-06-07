#version 330 core
out vec4 FragColor;

uniform vec3 objectColor;	//���屾�����ɫ
uniform vec3 lightColor;		//�ƹ���ɫ

void main()
{
	FragColor = vec4(lightColor * objectColor , 1.0f);
}