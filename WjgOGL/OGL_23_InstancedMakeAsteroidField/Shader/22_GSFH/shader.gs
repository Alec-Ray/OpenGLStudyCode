#version 330 core
layout (points) in;		//输入修饰符
layout (triangle_strip, max_vertices = 5) out;		//输出修饰符

in VS_OUT {
    vec3 color;
} gs_in[];

//设置片元着色器的颜色
out vec3 fColor;

void build_house(vec4 position)
{    
	//颜色设置一次就行了，除非你想让每个偏移点都有自己的颜色
    fColor = gs_in[0].color; 
    gl_Position = position + vec4(-0.2, -0.2, 0.0, 0.0); // 1:bottom-left   
    EmitVertex();   
    gl_Position = position + vec4( 0.2, -0.2, 0.0, 0.0); // 2:bottom-right
    EmitVertex();
    gl_Position = position + vec4(-0.2,  0.2, 0.0, 0.0); // 3:top-left
    EmitVertex();
    gl_Position = position + vec4( 0.2,  0.2, 0.0, 0.0); // 4:top-right
    EmitVertex();
    gl_Position = position + vec4( 0.0,  0.4, 0.0, 0.0); // 5:top
	//这里又设置一次是为了给顶部的点加一些特殊的颜色
    fColor = vec3(1.0, 1.0, 1.0);
    EmitVertex();
    EndPrimitive();
}

void main() {    
	build_house(gl_in[0].gl_Position);
}

//gl_in是OGL的一个内置变量
/*
每次我们调用EmitVertex时，gl_Position中的向量会被添加到图元中来。
当EndPrimitive被调用时，所有发射出的(Emitted)顶点都会合成为指定的输出渲染图元，这里是triangle_strip。
在一个或多个EmitVertex调用之后重复调用EndPrimitive能够生成多个图元。
在这个例子中，我们发射了5个顶点，(max_vertices = 5)，它们从原始顶点位置平移了一段距离，
之后调用了EndPrimitive，只要顺序正确，每够三个顶点就绘出一个三角形。
*/