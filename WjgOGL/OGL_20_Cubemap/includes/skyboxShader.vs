#version 330 core
layout (location = 0) in vec3 aPos;

out vec3 TexCoords;

uniform mat4 projection;
uniform mat4 view;

void main()
{
	//顶点着色器中很有意思的部分是，我们将输入的位置向量作为输出给片段着色器的纹理坐标
	//透视除法是在顶点着色器运行之后执行的，将gl_Position的xyz坐标除以w分量。
	//我们又从深度测试小节中知道，相除结果的z分量等于顶点的深度值。
	//使用这些信息，我们可以将输出位置的z分量等于它的w分量，让z分量永远等于1.0，
	//这样子的话，当透视除法执行之后，z分量会变为w / w = 1.0。
	TexCoords = aPos;
    vec4 pos = projection * view * vec4(aPos, 1.0);
    gl_Position = pos.xyww;
}