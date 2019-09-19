#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;

out vec2 TexCoords;

//帧缓冲最终生成的图像的的顶点数据，这些数据是屏幕空间下的数据，因此不需要空间变换
//uniform mat4 model;
//uniform mat4 view;
//uniform mat4 projection;

void main()
{  
    //gl_Position = projection * view * model * vec4(aPos, 1.0);

	TexCoords = aTexCoords;  
	gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0); 
}

//为了绘制这个四边形，我们将会新创建一套简单的着色器。
//我们将不会包含任何花哨的矩阵变换，因为我们提供的是标准化设备坐标的顶点坐标，
//所以我们可以直接将它们设定为顶点着色器的输出