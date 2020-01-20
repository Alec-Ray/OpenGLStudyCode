#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 model;

void main()
{
	//将顶点变换到世界空间，变换到光空间由几何着色器来做
    gl_Position = model * vec4(aPos, 1.0);
}