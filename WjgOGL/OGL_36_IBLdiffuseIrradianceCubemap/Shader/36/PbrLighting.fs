#version 330 core
out vec4 FragColor;
in vec2 TexCoords;
in vec3 WorldPos;
in vec3 Normal;

//������
uniform vec3 albedo;
//������
uniform float metallic;
//�����
uniform float roughness;
//��������
uniform float ao;

uniform vec3 lightPositions[4];
uniform vec3 lightColors[4];
uniform vec3 camPos;
const float PI = 3.14159265359;

// DNF(N,H,r) = r^2 / (PI * ((dot(N,H)^2) * (r^2 - 1)+ 1)^2))
// ���߷ֲ����� Normal Distribution Function���������м�����Hһ�µ�΢ƽ��ı���
// N:΢ƽ�淨�߷���H:�������Ƚϵ��м�������roughness�ֲڶ�
// H�ļ��㷽ʽ��ȡ�������������߷�����м�
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

// G(N,V,k) = dot(N,V) / (dot(N,V) * (1 - k) + k)
// ���κ��� Geometry Function ����΢ƽ���໥���ֵı���
// NdotV��˽����roughness�ֲڶ�
// Խ�ֲڵı�����΢ƽ��Ҳ��Խ���׻����ڱ�
float GeometrySchlickGGX(float NdotV, float roughness)
{
	float r = (roughness + 1.0);
	//����k��r���ֱ�ӹ��յ���ӳ��(Remapping)��IBL��ӳ����㷽ʽ��˲�ͬ
	float k = (r*r) / 8.0;
	float nom   = NdotV;
	float denom = NdotV * (1.0 - k) + k;

	return nom / denom;
}
//��ʷ��˹��(Smith method)�ѹ۲췽��͹��߷��򶼿��ǽ�ȥ
//N�����߷��� V:���߷��� L�����߷���
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
	float NdotV = max(dot(N, V), 0.0);
	float NdotL = max(dot(N, L), 0.0);
	// ���߷�����ڱθ���
	float ggx2 = GeometrySchlickGGX(NdotV, roughness);
	// ��Դ������ڱθ���
	float ggx1 = GeometrySchlickGGX(NdotL, roughness);

	return ggx1 * ggx2;
}

// F(h,v,F0) = F0 + (1 - F0) * ((1 - dot(h,v))^5)  
// ���������̣�fresnelSchlick��һ�ֽ��Ʒ������ص������������߱�����İٷֱ�
// cosTheta�Ƿ��߷�����۲췽��ĵ�ˣ������м�������۲췽��ĵ��
vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
	return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}

void main()
{		
	vec3 N = normalize(Normal);
	vec3 V = normalize(camPos - WorldPos);

	//�ǽ�������������ʹ��0.04�͹��ˣ������Ļ�ʹ������albedo��ͼ����ֵ
	//��Ϊ�����������������������߶�û�������䣬�������ǿ���ֱ��ʹ������albeto������ֵ����Ϊ���Ļ���������
	vec3 F0 = vec3(0.04); 
	F0 = mix(F0, albedo, metallic);

	// �ⷽ�����
	// �����ն�Lo(Irradiance)
	vec3 Lo = vec3(0.0);
	for(int i = 0; i < 4; ++i) 
	{
		// ����ÿһյ�Ƶķ�����(Radiance)��Ȼ�����BRDF�͹�Դ������������Ŵ˷�����
		vec3 L = normalize(lightPositions[i] - WorldPos);
		vec3 H = normalize(V + L);
		float distance = length(lightPositions[i] - WorldPos);
		float attenuation = 1.0 / (distance * distance);
		vec3 radiance = lightColors[i] * attenuation;

		// Cook-Torrance BRDFģ�� DFG / (4 * dot(Wo,n) * dot(Wi,n)) 
		//Wo�۲췽��Ҳ���ǳ��䷽��  Wi�ƹⷽ��Ҳ�������䷽��
		float NDF = DistributionGGX(N, H, roughness);				//D
		float G   = GeometrySmith(N, V, L, roughness);				//G    
		vec3 F    = fresnelSchlick(clamp(dot(H, V), 0.0, 1.0), F0); //F     
		vec3 nominator    = NDF * G * F; 
		float denominator = 4 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0);
		//��0.001��Ϊ�˱�����С��0
		vec3 specular = nominator / max(denominator, 0.001); 

		// ���߷�����ռ�ٷֱ�
		vec3 kS = F;
		// ����������ռ�ٷֱȣ�ע�������غ�
		vec3 kD = vec3(1.0) - kS;
		//��kD���Է������ԣ�ʹ��ֻ�зǽ����ž���������������������ǲ��ֽ�����Ϊ���Ի�ϣ�������û������⣩��
		kD *= 1.0 - metallic;	  

		float NdotL = max(dot(N, L), 0.0);        

		// ���հ淽��
		// ע������specular��û����ʽ��д��������ks����Ϊ���ǳ��Է��������൱�ڳ˹���ks
		Lo += (kD * albedo / PI + specular) * radiance * NdotL;  
	}   

	//��һ��СС�Ļ�����
	vec3 ambient = vec3(0.03) * albedo * ao;
	vec3 color = ambient + Lo;

	// HDRɫ��ӳ��
	color = color / (color + vec3(1.0));
	// gammaУ��
	color = pow(color, vec3(1.0/2.2)); 

	FragColor = vec4(color, 1.0);
}
