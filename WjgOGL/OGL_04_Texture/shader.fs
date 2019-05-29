#version 330 core
out vec4 FragColor;

in vec3 ourColor;	// 从顶点着色器传来的输入变量(名称相同，类型相同)
in vec2 TexCoord;   // 从顶点着色器传来的纹理坐标

uniform sampler2D ourTexture;	//声明一个uniform sampler2D把一个纹理添加到片段着色器中

void main()
{
    //FragColor = vec4(ourColor, 1.0f);

	//使用GLSL内建的texture函数来采样纹理的颜色
	//它第一个参数是纹理采样器，第二个参数是对应的纹理坐标
	FragColor = texture(ourTexture,TexCoord);
}

