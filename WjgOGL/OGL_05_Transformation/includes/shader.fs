#version 330 core
out vec4 FragColor;

in vec3 ourColor;	// 从顶点着色器传来的输入变量(名称相同，类型相同)
in vec2 TexCoord;   // 从顶点着色器传来的纹理坐标

uniform sampler2D texture01;	//纹理1
uniform sampler2D texture02;	//纹理2

void main()
{
    //FragColor = vec4(ourColor, 1.0f);

	//使用GLSL内建的texture函数来采样纹理的颜色
	//它第一个参数是纹理采样器，第二个参数是对应的纹理坐标
	//FragColor = texture(ourTexture,TexCoord);

	//最终输出颜色现在是两个纹理的结合
	//如果第三个值是0.0，它会返回第一个输入；
	//如果是1.0，会返回第二个输入值
	//0.2会返回80%的第一个输入颜色和20%的第二个输入颜色，即返回两个纹理的混合色。
	FragColor = mix(texture(texture01,TexCoord),texture(texture02,TexCoord),0.4);
}

