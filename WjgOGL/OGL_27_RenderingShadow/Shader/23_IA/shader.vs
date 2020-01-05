#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aOffset;

out vec3 fColor;

void main()
{
    gl_Position = vec4(aPos * (gl_InstanceID / 100.0) + aOffset, 0.0, 1.0);
    fColor = aColor;
}

/*
顶点着色器的内建变量，gl_InstanceID
在使用实例化渲染调用时，gl_InstanceID会从0开始，在每个实例被渲染时递增
aPos * (gl_InstanceID / 100.0),从右上到左下逐渐缩小四边形
*/