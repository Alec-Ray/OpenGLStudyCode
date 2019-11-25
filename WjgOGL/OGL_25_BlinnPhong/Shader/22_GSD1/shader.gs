#version 330 core
layout (points) in;		//输入修饰符
layout (line_strip, max_vertices = 2) out;		//输出修饰符

void main() {    
    gl_Position = gl_in[0].gl_Position + vec4(-0.1, 0.0, 0.0, 0.0); 
    EmitVertex();

    gl_Position = gl_in[0].gl_Position + vec4( 0.1, 0.0, 0.0, 0.0);
    EmitVertex();

    EndPrimitive();
}

//gl_in是OGL的一个内置变量
/*
每次我们调用EmitVertex时，gl_Position中的向量会被添加到图元中来。
当EndPrimitive被调用时，所有发射出的(Emitted)顶点都会合成为指定的输出渲染图元，这里是line_strip。
在一个或多个EmitVertex调用之后重复调用EndPrimitive能够生成多个图元。
在这个例子中，我们发射了两个顶点，(max_vertices = 2)，它们从原始顶点位置平移了一段距离，
之后调用了EndPrimitive，将这两个顶点合成为一个包含两个顶点的线条。
*/