#version 330 core
out vec4 FragColor;

uniform vec3 objectColor;	//物体本身的颜色
uniform vec3 lightColor;		//灯光颜色

uniform vec3 lightPos;		//光源的位置，这里是固定值
uniform vec3 viewPos;		//观察者坐标，这里观察者就是摄像机

in vec3 Normal;				//接受顶点着色器传递来的法向量
in vec3 FragPos;			//此片段在世界空间下的坐标

in vec2 TexCoords;

//材质
struct Material {
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};

uniform Material material;

//光源对ambient、diffuse和specular光照有着不同的强度
struct Light {
	vec3 position;	

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	//点光源衰减公式的三个参数
	float constant;
    float linear;
    float quadratic;
};

uniform Light light;

void main()
{
	//将环境光得材质颜色设置为漫反射材质颜色同样的值
	vec3 ambient = light.ambient * vec3(texture(material.diffuse,TexCoords));

	//漫反射
	vec3 norm = normalize(Normal);														//标准化法向量
	vec3 lightDir = normalize(lightPos - FragPos);										//标准化光线的方向

	float diff = max(dot(norm,lightDir),0.0);											//点乘计算漫反射分量，max是防止diff小于0
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse,TexCoords));	//最终的漫反射颜色

	//镜面反射
	vec3 viewDir = normalize(viewPos - FragPos);									//视线向量
	vec3 reflectDir = normalize(reflect(-lightDir,norm));							//反射向量
	float spec = pow(max(dot(viewDir,reflectDir),0.0),material.shininess);			//计算镜面分量，这个32是高光的反光度
	vec3 specular = light.specular * spec * vec3(texture(material.specular , TexCoords));						//最终的高光颜色

	//点光源距离衰减值
	float distance    = length(lightPos - FragPos);		//片段距离点光源的距离
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

	vec3 result = (ambient  + diffuse  + specular) * attenuation;	
	FragColor = vec4(result , 1.0f);
}