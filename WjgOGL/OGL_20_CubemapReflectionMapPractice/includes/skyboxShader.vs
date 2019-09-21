#version 330 core
layout (location = 0) in vec3 aPos;

out vec3 TexCoords;

uniform mat4 projection;
uniform mat4 view;

void main()
{
	//顶点着色器中很有意思的部分是，我们将输入的位置向量作为输出给片段着色器的纹理坐标
    //TexCoords = aPos;
    //gl_Position = projection * view * vec4(aPos, 1.0);
	TexCoords = aPos;
    vec4 pos = projection * view * vec4(aPos, 1.0);
    gl_Position = pos.xyww;
}