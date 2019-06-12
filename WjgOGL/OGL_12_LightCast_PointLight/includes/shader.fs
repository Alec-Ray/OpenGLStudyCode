#version 330 core
out vec4 FragColor;

uniform vec3 objectColor;	//���屾�����ɫ
uniform vec3 lightColor;		//�ƹ���ɫ

uniform vec3 lightPos;		//��Դ��λ�ã������ǹ̶�ֵ
uniform vec3 viewPos;		//�۲������꣬����۲��߾��������

in vec3 Normal;				//���ܶ�����ɫ���������ķ�����
in vec3 FragPos;			//��Ƭ��������ռ��µ�����

in vec2 TexCoords;

//����
struct Material {
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};

uniform Material material;

//��Դ��ambient��diffuse��specular�������Ų�ͬ��ǿ��
struct Light {
	vec3 position;	

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	//���Դ˥����ʽ����������
	float constant;
    float linear;
    float quadratic;
};

uniform Light light;

void main()
{
	//��������ò�����ɫ����Ϊ�����������ɫͬ����ֵ
	vec3 ambient = light.ambient * vec3(texture(material.diffuse,TexCoords));

	//������
	vec3 norm = normalize(Normal);														//��׼��������
	vec3 lightDir = normalize(lightPos - FragPos);										//��׼�����ߵķ���

	float diff = max(dot(norm,lightDir),0.0);											//��˼��������������max�Ƿ�ֹdiffС��0
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse,TexCoords));	//���յ���������ɫ

	//���淴��
	vec3 viewDir = normalize(viewPos - FragPos);									//��������
	vec3 reflectDir = normalize(reflect(-lightDir,norm));							//��������
	float spec = pow(max(dot(viewDir,reflectDir),0.0),material.shininess);			//���㾵����������32�Ǹ߹�ķ����
	vec3 specular = light.specular * spec * vec3(texture(material.specular , TexCoords));						//���յĸ߹���ɫ

	//���Դ����˥��ֵ
	float distance    = length(lightPos - FragPos);		//Ƭ�ξ�����Դ�ľ���
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

	vec3 result = (ambient  + diffuse  + specular) * attenuation;	
	FragColor = vec4(result , 1.0f);
}