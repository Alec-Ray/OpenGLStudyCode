#version 330 core
out vec4 FragColor;
in vec2 TexCoords;
in vec3 WorldPos;
in vec3 Normal;

uniform vec3 albedo;
uniform float metallic;
uniform float roughness;
uniform float ao;

uniform vec3 lightPositions[4];
uniform vec3 lightColors[4];
uniform vec3 camPos;
const float PI = 3.14159265359;

// Ϊ����Ԥ����õķ��ն�ͼ���Ϊһ�������������
uniform samplerCube irradianceMap;

float DistributionGGX(vec3 N, vec3 H, float roughness)
{
	float a = roughness*roughness;
	float a2 = a*a;
	float NdotH = max(dot(N, H), 0.0);
	float NdotH2 = NdotH*NdotH;

	float nom   = a2;
	float denom = (NdotH2 * (a2 - 1.0) + 1.0);
	denom = PI * denom * denom;

	return nom / max(denom, 0.001); 
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
	float r = (roughness + 1.0);
	float k = (r*r) / 8.0;
	float nom   = NdotV;
	float denom = NdotV * (1.0 - k) + k;

	return nom / denom;
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
	float NdotV = max(dot(N, V), 0.0);
	float NdotL = max(dot(N, L), 0.0);
	float ggx2 = GeometrySchlickGGX(NdotV, roughness);
	float ggx1 = GeometrySchlickGGX(NdotL, roughness);

	return ggx1 * ggx2;
}

vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
	return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}

//�����˴ֲڶȵķ���������
vec3 fresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness)
{
	return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(1.0 - cosTheta, 5.0);
}   

void main()
{		
	vec3 N = normalize(Normal);
	vec3 V = normalize(camPos - WorldPos);

	vec3 F0 = vec3(0.04); 
	F0 = mix(F0, albedo, metallic);

	vec3 Lo = vec3(0.0);
	for(int i = 0; i < 4; ++i) 
	{
		vec3 L = normalize(lightPositions[i] - WorldPos);
		vec3 H = normalize(V + L);
		float distance = length(lightPositions[i] - WorldPos);
		float attenuation = 1.0 / (distance * distance);
		vec3 radiance = lightColors[i] * attenuation;

		float NDF = DistributionGGX(N, H, roughness);				
		float G   = GeometrySmith(N, V, L, roughness);				
		vec3 F    = fresnelSchlick(clamp(dot(H, V), 0.0, 1.0), F0);   
		vec3 nominator    = NDF * G * F; 
		float denominator = 4 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0);
		vec3 specular = nominator / max(denominator, 0.001); 

		vec3 kS = F;
		vec3 kD = vec3(1.0) - kS;
		kD *= 1.0 - metallic;	  

		float NdotL = max(dot(N, L), 0.0);        

		Lo += (kD * albedo / PI + specular) * radiance * NdotL;  
	}   

	// �ӷ��ն���������ͼȡ�����նȣ����㻷����
	// ��Ϊ��ӹ����������;��淴�䣬�����������÷������ʽ�����������Ȩ��
	//vec3 ambient = vec3(0.03) * albedo * ao;
	vec3 kS = fresnelSchlick(max(dot(N, V), 0.0), F0);
	//vec3 kS = fresnelSchlickRoughness(max(dot(N, V), 0.0), F0, roughness);
	vec3 kD = 1.0 - kS;
	kD *= 1.0 - metallic;	 
	//����һ�ŷ��ն�ͼ�����洢�˳����е����м��������⣬��ȡƬ�εķ��նȾͼ�Ϊ�������ߵ�һ�������������������������(������̫��)
	vec3 irradiance = texture(irradianceMap, N).rgb;
	vec3 diffuse      = irradiance * albedo;
	vec3 ambient = (kD * diffuse) * ao;

	vec3 color = ambient + Lo;

	color = color / (color + vec3(1.0));
	color = pow(color, vec3(1.0/2.2)); 

	FragColor = vec4(color, 1.0);
}
