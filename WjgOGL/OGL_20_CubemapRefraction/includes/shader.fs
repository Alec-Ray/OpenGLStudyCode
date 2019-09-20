#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 Position;

uniform vec3 cameraPos;
//samplerCube��������ͼ
uniform samplerCube skybox;

//�����Ӻͽ�ɫ��������
void main()
{    
	//�������ʹ��GLSL���ڽ�refract����������ʵ�֣�
	//����Ҫһ����������һ���۲췽�����������֮���������(Refractive Index)��
    //���ǵ������У�����/���ߴӿ������벣��
	//��������Ǽ��������ǲ����Ƶģ������Ա�ֵΪ1.00/1.52=0.658
	float ratio = 1.00 / 1.52;
    vec3 I = normalize(Position - cameraPos);
    vec3 R = refract(I, normalize(Normal), ratio);
    FragColor = vec4(texture(skybox, R).rgb, 1.0);
}