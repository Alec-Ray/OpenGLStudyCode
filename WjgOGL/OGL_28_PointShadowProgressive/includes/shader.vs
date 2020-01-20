#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;

out vec2 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    TexCoords = aTexCoords;    
    gl_Position = projection * view * model * vec4(aPos, 1.0);
 
	//可以通过OpenGL的glPointSize函数来设置渲染出来的点的大小，
	//但我们也可以在顶点着色器中修改这个值
	//将点的大小设置为裁剪空间位置的z值，也就是顶点距观察者的距离。
	//点的大小会随着观察者距顶点距离变远而增大。
    gl_PointSize = gl_Position.z;  
}