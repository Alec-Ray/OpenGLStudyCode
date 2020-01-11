#version 330 core
layout (triangles) in;
layout (triangle_strip, max_vertices=18) out;

uniform mat4 shadowMatrices[6];

out vec4 FragPos;

void main()
{
    for(int face = 0; face < 6; ++face)
    {
		//gl_Layer，它指定发散出基本图形送到立方体贴图的哪个面
        gl_Layer = face;
		//输入一个三角形，输出总共6个三角形（6*3顶点，所以总共18个顶点）
        for(int i = 0; i < 3; ++i) 
        {
            FragPos = gl_in[i].gl_Position;
			//把面的光空间变换矩阵乘以FragPos，将每个世界空间顶点变换到相关的光空间，生成每个三角形
            gl_Position = shadowMatrices[face] * FragPos;
            EmitVertex();
        }    
        EndPrimitive();
    }
} 