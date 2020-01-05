#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec3 aColor;

out vec3 fColor;

//存了100个偏移量
uniform vec2 offsets[100];

void main()
{
    vec2 offset = offsets[gl_InstanceID];
    gl_Position = vec4(aPos + offset, 0.0, 1.0);
    fColor = aColor;
}

/*
顶点着色器的内建变量，gl_InstanceID
在使用实例化渲染调用时，gl_InstanceID会从0开始，在每个实例被渲染时递增
*/