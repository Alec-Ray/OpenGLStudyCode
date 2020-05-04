#version 330 core
layout (location = 0) in vec3 aPos;

//将局部坐标作为3D采样向量传给片段着色器
out vec3 WorldPos;

uniform mat4 projection;
uniform mat4 view;

void main()
{
    WorldPos = aPos;
    gl_Position =  projection * view * vec4(WorldPos, 1.0);
}