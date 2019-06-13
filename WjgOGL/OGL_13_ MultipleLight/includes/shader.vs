#version 330 core
layout (location = 0) in vec3 aPos;			// 位置变量的属性位置值为 0 
layout (location = 1) in vec3 aNormal;      // 顶点的法向量的属性位置值为1
layout (location = 2) in vec2 aTexCoords;	//纹理坐标

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 Normal;			//将法向量由顶点着色器传递给片段着色器
out vec3 FragPos;			//将此顶点的位置传递给片段着色器，注意这里传的是世界坐标下的坐标,所以只乘个模型矩阵

out vec2 TexCoords;

void main()
{
	//注意矩阵从右往左乘	
	gl_Position = projection * view * model * vec4(aPos,1.0f);

	//Normal = aNormal;
	//使用法线矩阵乘以法向量以应对模型的不等比缩放
	Normal = mat3(transpose(inverse(model))) * aNormal;

	TexCoords = aTexCoords;

	FragPos = vec3 (model * vec4 (aPos,1.0f));
}