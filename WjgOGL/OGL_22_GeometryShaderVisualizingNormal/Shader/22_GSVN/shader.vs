#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out VS_OUT {
    vec3 normal;
} vs_out;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main()
{
    mat3 normalMatrix = mat3(transpose(inverse(view * model)));
    vs_out.normal = vec3(projection * vec4(normalMatrix * aNormal, 0.0));
    gl_Position = projection * view * model * vec4(aPos, 1.0); 
}

/*
为了适配（观察和模型矩阵的）缩放和旋转，我们在将法线变换到裁剪空间坐标之前，
先使用法线矩阵变换一次（几何着色器接受的位置向量是剪裁空间坐标，
所以我们应该将法向量变换到相同的空间中；
变换后的裁剪空间法向量会以接口块的形式传递到下个着色器阶段。
接下来，几何着色器会接收每一个顶点（包括一个位置向量和一个法向量），
并在每个位置向量处绘制一个法线向量
*/