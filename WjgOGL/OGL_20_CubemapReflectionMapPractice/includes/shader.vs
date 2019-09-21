#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec2 TexCoords;

out vec3 Normal;
out vec3 Position;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    TexCoords = aTexCoords;    

	//使用法线矩阵(Normal Matrix)来变换法线向量
    Normal = mat3(transpose(inverse(model))) * aNormal;
	//Position输出向量是一个世界空间的位置向量
    Position = vec3(model * vec4(aPos, 1.0)); 
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}