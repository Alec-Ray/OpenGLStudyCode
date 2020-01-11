#version 330 core
out vec4 FragColor;

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
    vec4 FragPosLightSpace;
} fs_in;

uniform sampler2D diffuseTexture;
uniform sampler2D shadowMap;

uniform vec3 lightPos;
uniform vec3 viewPos;

float ShadowCalculation(vec4 fragPosLightSpace)
{
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;
    float closestDepth = texture(shadowMap, projCoords.xy).r; 
    float currentDepth = projCoords.z;

	//����1.ʧ�棬�����κδ���
	//float shadow = currentDepth > closestDepth  ? 1.0 : 0.0;
	
	//����2.ʧ�棬Ӧ��һ��bias�̶�ƫ�������Ϳ��Եõ�һ�������Ч��
    //float shadow = (currentDepth - bias) > closestDepth  ? 1.0 : 0.0;

	//����3.ʧ�棬bias�����̶�����Щ�����¶Ⱥܴ���Ȼ�������Ӱʧ�档���ݱ��泯����ߵĽǶȸ���ƫ������ʹ�õ��
	//vec3 normal = normalize(fs_in.Normal);
	//vec3 lightDir = normalize(lightPos - fs_in.FragPos);
	//float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);
	//float shadow = (currentDepth - bias) > closestDepth  ? 1.0 : 0.0;

	//����4������ʧ�� + ���
	//PCF��percentage-closer filtering��������9��ֵƽ��
	float shadow = 0.0;
	vec3 normal = normalize(fs_in.Normal);
	vec3 lightDir = normalize(lightPos - fs_in.FragPos);
	float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);
	vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
	for(int x = -1; x <= 1; ++x)
	{
		for(int y = -1; y <= 1; ++y)
		{
			float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r; 
			shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;        
		}    
	}
	shadow /= 9.0;

	//ֻҪͶӰ������z�������1.0�����ǾͰ�shadow��ֵǿ����Ϊ0.0,���������׶��Զƽ��Zֵ����1��Ϊ����Ӱ���������
	if(projCoords.z > 1.0)
		shadow = 0.0;

    return shadow;
}

void main()
{           
    vec3 color = texture(diffuseTexture, fs_in.TexCoords).rgb;
    vec3 normal = normalize(fs_in.Normal);
    vec3 lightColor = vec3(0.3);
    // ambient
    vec3 ambient = 0.3 * color;
    // diffuse
    vec3 lightDir = normalize(lightPos - fs_in.FragPos);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * lightColor;
    // specular
    vec3 viewDir = normalize(viewPos - fs_in.FragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = 0.0;
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    spec = pow(max(dot(normal, halfwayDir), 0.0), 64.0);
    vec3 specular = spec * lightColor;    

    float shadow = ShadowCalculation(fs_in.FragPosLightSpace);                      
    vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular)) * color;    
    
    FragColor = vec4(lighting, 1.0);
}