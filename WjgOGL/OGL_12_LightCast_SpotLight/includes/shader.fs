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
	vec3 position;		//�۹�Ƶ�λ������
	vec3 direction;		//�۹�Ƶķ�������
	float cutOff;		//�۹�Ƶ��й��

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

uniform Light light;

void main()
{
	vec3 lightDir = normalize(light.position - FragPos);								//��׼����Դ��㵽Ƭ�εķ���
	
	//LightDir������SpotDir����֮��ļн�,�ھ۹��ڲ��Ļ���ֵӦ�ñ�?ֵС��
	float theta = dot(lightDir,normalize(-light.direction));

	//�Ƕ�ֵ���ڶ�������ֵ����ʾ��,�����ô��ں�
	if(theta > light.cutOff)
	{
		//��������ò�����ɫ����Ϊ�����������ɫͬ����ֵ
		vec3 ambient = light.ambient * vec3(texture(material.diffuse,TexCoords));

		//������
		vec3 norm = normalize(Normal);														//��׼��������
		float diff = max(dot(norm,lightDir),0.0);											//��˼��������������max�Ƿ�ֹdiffС��0
		vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse,TexCoords));	//���յ���������ɫ

		//���淴��
		vec3 viewDir = normalize(viewPos - FragPos);									//��������
		vec3 reflectDir = normalize(reflect(-lightDir,norm));							//��������
		float spec = pow(max(dot(viewDir,reflectDir),0.0),material.shininess);			//���㾵����������32�Ǹ߹�ķ����
		vec3 specular = light.specular * spec * vec3(texture(material.specular , TexCoords));						//���յĸ߹���ɫ

		//vec3 result = ambient * objectColor + diffuse * objectColor + specular * objectColor;	
		vec3 result = ambient  + diffuse  + specular;	
		FragColor = vec4(result , 1.0f);
	}
	else
	{
		//������ھ۹�Ʒ�Χ�ڣ�Ϊ����ģ��Ϊ��ɫ������ֻ��һ��������
		FragColor = vec4(light.ambient * texture(material.diffuse, TexCoords).rgb, 1.0);
		//FragColor =vec4(1.0f,0.0f,0.0f,1.0f);
	}
}