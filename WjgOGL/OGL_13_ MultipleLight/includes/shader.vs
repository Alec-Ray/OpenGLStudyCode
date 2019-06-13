#version 330 core
layout (location = 0) in vec3 aPos;			// λ�ñ���������λ��ֵΪ 0 
layout (location = 1) in vec3 aNormal;      // ����ķ�����������λ��ֵΪ1
layout (location = 2) in vec2 aTexCoords;	//��������

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 Normal;			//���������ɶ�����ɫ�����ݸ�Ƭ����ɫ��
out vec3 FragPos;			//���˶����λ�ô��ݸ�Ƭ����ɫ����ע�����ﴫ�������������µ�����,����ֻ�˸�ģ�;���

out vec2 TexCoords;

void main()
{
	//ע�������������	
	gl_Position = projection * view * model * vec4(aPos,1.0f);

	//Normal = aNormal;
	//ʹ�÷��߾�����Է�������Ӧ��ģ�͵Ĳ��ȱ�����
	Normal = mat3(transpose(inverse(model))) * aNormal;

	TexCoords = aTexCoords;

	FragPos = vec3 (model * vec4 (aPos,1.0f));
}