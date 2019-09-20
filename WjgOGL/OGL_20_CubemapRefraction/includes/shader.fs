#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 Position;

uniform vec3 cameraPos;
//samplerCube立方体贴图
uniform samplerCube skybox;

//让箱子和角色具有折射
void main()
{    
	//折射可以使用GLSL的内建refract函数来轻松实现，
	//它需要一个法向量、一个观察方向和两个材质之间的折射率(Refractive Index)。
    //我们的例子中，光线/视线从空气进入玻璃
	//（如果我们假设箱子是玻璃制的），所以比值为1.00/1.52=0.658
	float ratio = 1.00 / 1.52;
    vec3 I = normalize(Position - cameraPos);
    vec3 R = refract(I, normalize(Normal), ratio);
    FragColor = vec4(texture(skybox, R).rgb, 1.0);
}