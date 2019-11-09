#version 330 core
layout (location = 0) in vec3 aPos;

//投影矩阵和观察者矩阵基本不会变，所以放到uniform缓冲当中
layout (std140) uniform Matrices
{
	mat4 projection;
	mat4 view;
};

//模型矩阵在不同的着色器中不一样，所以单独拿出来
uniform mat4 model;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}  
