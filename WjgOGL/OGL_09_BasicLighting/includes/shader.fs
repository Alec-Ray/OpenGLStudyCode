#version 330 core
out vec4 FragColor;

uniform vec3 objectColor;	//物体本身的颜色
uniform vec3 lightColor;		//灯光颜色

uniform vec3 lightPos;		//光源的位置，这里是固定值
uniform vec3 viewPos;		//观察者坐标，这里观察者就是摄像机

in vec3 Normal;				//接受顶点着色器传递来的法向量
in vec3 FragPos;			//此片段在世界空间下的坐标

void main()
{
	//环境光
	float ambientStrength = 0.1;					//环境光系数
	vec3 ambient = lightColor * ambientStrength;

	//漫反射
	vec3 norm = normalize(Normal);					//标准化法向量
	vec3 lightDir = normalize(lightPos - FragPos);	//标准化光线的方向
	float diff = max(dot(norm,lightDir),0.0);		//点乘计算漫反射分量，max是防止diff小于0
	vec3 diffuse = diff * lightColor;				//最终的漫反射颜色

	//镜面反射
	float specularStrength = 0.5;							//镜面强度
	vec3 viewDir = normalize(viewPos - FragPos);			//视线向量
	vec3 reflectDir = normalize(reflect(-lightDir,norm));	//反射向量
	float spec = pow(max(dot(viewDir,reflectDir),0.0),32);	//计算镜面分量，这个32是高光的反光度
	vec3 specular = specularStrength * spec * lightColor;	//最终的高光颜色

	vec3 result = ambient * objectColor + diffuse * objectColor + specular * objectColor;	//最终颜色
	FragColor = vec4(result , 1.0f);
}