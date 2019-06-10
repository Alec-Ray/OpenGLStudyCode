#version 330 core
out vec4 FragColor;

uniform vec3 objectColor;	//���屾�����ɫ
uniform vec3 lightColor;		//�ƹ���ɫ

uniform vec3 lightPos;		//��Դ��λ�ã������ǹ̶�ֵ
uniform vec3 viewPos;		//�۲������꣬����۲��߾��������

in vec3 Normal;				//���ܶ�����ɫ���������ķ�����
in vec3 FragPos;			//��Ƭ��������ռ��µ�����

//����
struct Material {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

uniform Material material;

//��Դ��ambient��diffuse��specular�������Ų�ͬ��ǿ��
struct Light {
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

uniform Light light;

void main()
{
	//������
	vec3 ambient = light.ambient * material.ambient;

	//������
	vec3 norm = normalize(Normal);					//��׼��������
	vec3 lightDir = normalize(lightPos - FragPos);	//��׼�����ߵķ���
	float diff = max(dot(norm,lightDir),0.0);		//��˼��������������max�Ƿ�ֹdiffС��0
	vec3 diffuse = light.diffuse * diff * material.diffuse;				//���յ���������ɫ

	//���淴��
	vec3 viewDir = normalize(viewPos - FragPos);			//��������
	vec3 reflectDir = normalize(reflect(-lightDir,norm));	//��������
	float spec = pow(max(dot(viewDir,reflectDir),0.0),material.shininess);	//���㾵����������32�Ǹ߹�ķ����
	vec3 specular = light.specular * material.specular * spec;	//���յĸ߹���ɫ

	//vec3 result = ambient * objectColor + diffuse * objectColor + specular * objectColor;	
	vec3 result = ambient  + diffuse  + specular;	
	FragColor = vec4(result , 1.0f);
}