#version 330 core
out vec4 FragColor;
in vec2 TexCoords;
in vec3 WorldPos;
in vec3 Normal;

//反照率
uniform vec3 albedo;
//金属度
uniform float metallic;
//粗燥度
uniform float roughness;
//环境遮罩
uniform float ao;

uniform vec3 lightPositions[4];
uniform vec3 lightColors[4];
uniform vec3 camPos;
const float PI = 3.14159265359;

// DNF(N,H,r) = r^2 / (PI * ((dot(N,H)^2) * (r^2 - 1)+ 1)^2))
// 法线分布函数 Normal Distribution Function，计算与中间向量H一致的微平面的比率
// N:微平面法线方向，H:用来做比较的中间向量，roughness粗糙度
// H的计算方式，取光线向量和视线方向的中间
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
// 几何函数 Geometry Function 计算微平面相互遮罩的比率
// NdotV点乘结果，roughness粗糙度
// 越粗糙的表面其微平面也就越容易互相遮蔽
float GeometrySchlickGGX(float NdotV, float roughness)
{
	float r = (roughness + 1.0);
	//这里k是r针对直接光照的重映射(Remapping)，IBL重映射计算方式与此不同
	float k = (r*r) / 8.0;
	float nom   = NdotV;
	float denom = NdotV * (1.0 - k) + k;

	return nom / denom;
}
//用史密斯法(Smith method)把观察方向和光线方向都考虑进去
//N：法线方向 V:视线方向 L：光线方向
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
	float NdotV = max(dot(N, V), 0.0);
	float NdotL = max(dot(N, L), 0.0);
	// 视线方向的遮蔽概率
	float ggx2 = GeometrySchlickGGX(NdotV, roughness);
	// 光源方向的遮蔽概率
	float ggx1 = GeometrySchlickGGX(NdotL, roughness);

	return ggx1 * ggx2;
}

// F(h,v,F0) = F0 + (1 - F0) * ((1 - dot(h,v))^5)  
// 菲涅尔方程，fresnelSchlick是一种近似法，返回的是物体表面光线被反射的百分比
// cosTheta是法线方向与观察方向的点乘，或者中间向量与观察方向的点乘
vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
	return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}

void main()
{		
	vec3 N = normalize(Normal);
	vec3 V = normalize(camPos - WorldPos);

	//非金属反射率我们使用0.04就够了，金属的话使用它的albedo贴图的数值
	//因为金属表面会吸收所有折射光线而没有漫反射，所以我们可以直接使用它的albeto纹理数值来作为他的基础反射率
	vec3 F0 = vec3(0.04); 
	F0 = mix(F0, albedo, metallic);

	// 解方程组吧
	// 辐射照度Lo(Irradiance)
	vec3 Lo = vec3(0.0);
	for(int i = 0; i < 4; ++i) 
	{
		// 计算每一盏灯的辐射率(Radiance)，然后根据BRDF和光源的入射角来缩放此辐射率
		vec3 L = normalize(lightPositions[i] - WorldPos);
		vec3 H = normalize(V + L);
		float distance = length(lightPositions[i] - WorldPos);
		float attenuation = 1.0 / (distance * distance);
		vec3 radiance = lightColors[i] * attenuation;

		// Cook-Torrance BRDF模型 DFG / (4 * dot(Wo,n) * dot(Wi,n)) 
		//Wo观察方向，也就是出射方向  Wi灯光方向，也就是入射方向
		float NDF = DistributionGGX(N, H, roughness);				//D
		float G   = GeometrySmith(N, V, L, roughness);				//G    
		vec3 F    = fresnelSchlick(clamp(dot(H, V), 0.0, 1.0), F0); //F     
		vec3 nominator    = NDF * G * F; 
		float denominator = 4 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0);
		//加0.001是为了避免结果小于0
		vec3 specular = nominator / max(denominator, 0.001); 

		// 光线反射所占百分比
		vec3 kS = F;
		// 光线折射所占百分比，注意能量守恒
		vec3 kD = vec3(1.0) - kS;
		//将kD乘以反金属性，使得只有非金属才具有漫射照明，或者如果是部分金属则为线性混合（纯金属没有漫射光）。
		kD *= 1.0 - metallic;	  

		float NdotL = max(dot(N, L), 0.0);        

		// 最终版方程
		// 注意这里specular并没有像公式里写的那样乘ks，因为我们乘以菲涅尔就相当于乘过了ks
		Lo += (kD * albedo / PI + specular) * radiance * NdotL;  
	}   

	//加一个小小的环境光
	vec3 ambient = vec3(0.03) * albedo * ao;
	vec3 color = ambient + Lo;

	// HDR色调映射
	color = color / (color + vec3(1.0));
	// gamma校正
	color = pow(color, vec3(1.0/2.2)); 

	FragColor = vec4(color, 1.0);
}
