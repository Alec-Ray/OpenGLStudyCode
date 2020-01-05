#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 lightSpaceMatrix;
uniform mat4 model;

void main()
{
	//将顶点变换到光源空间中
    gl_Position = lightSpaceMatrix * model * vec4(aPos, 1.0);
}