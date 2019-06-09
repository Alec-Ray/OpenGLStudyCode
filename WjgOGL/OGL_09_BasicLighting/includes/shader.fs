#version 330 core
out vec4 FragColor;

uniform vec3 objectColor;	//���屾�����ɫ
uniform vec3 lightColor;		//�ƹ���ɫ

uniform vec3 lightPos;		//��Դ��λ�ã������ǹ̶�ֵ
uniform vec3 viewPos;		//�۲������꣬����۲��߾��������

in vec3 Normal;				//���ܶ�����ɫ���������ķ�����
in vec3 FragPos;			//��Ƭ��������ռ��µ�����

void main()
{
	//������
	float ambientStrength = 0.1;					//������ϵ��
	vec3 ambient = lightColor * ambientStrength;

	//������
	vec3 norm = normalize(Normal);					//��׼��������
	vec3 lightDir = normalize(lightPos - FragPos);	//��׼�����ߵķ���
	float diff = max(dot(norm,lightDir),0.0);		//��˼��������������max�Ƿ�ֹdiffС��0
	vec3 diffuse = diff * lightColor;				//���յ���������ɫ

	//���淴��
	float specularStrength = 0.5;							//����ǿ��
	vec3 viewDir = normalize(viewPos - FragPos);			//��������
	vec3 reflectDir = normalize(reflect(-lightDir,norm));	//��������
	float spec = pow(max(dot(viewDir,reflectDir),0.0),32);	//���㾵����������32�Ǹ߹�ķ����
	vec3 specular = specularStrength * spec * lightColor;	//���յĸ߹���ɫ

	vec3 result = ambient * objectColor + diffuse * objectColor + specular * objectColor;	//������ɫ
	FragColor = vec4(result , 1.0f);
}