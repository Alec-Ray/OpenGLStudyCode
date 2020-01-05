#version 330 core
layout (points) in;		//�������η�
layout (triangle_strip, max_vertices = 5) out;		//������η�

in VS_OUT {
    vec3 color;
} gs_in[];

//����ƬԪ��ɫ������ɫ
out vec3 fColor;

void build_house(vec4 position)
{    
	//��ɫ����һ�ξ����ˣ�����������ÿ��ƫ�Ƶ㶼���Լ�����ɫ
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
	//����������һ����Ϊ�˸������ĵ��һЩ�������ɫ
    fColor = vec3(1.0, 1.0, 1.0);
    EmitVertex();
    EndPrimitive();
}

void main() {    
	build_house(gl_in[0].gl_Position);
}

//gl_in��OGL��һ�����ñ���
/*
ÿ�����ǵ���EmitVertexʱ��gl_Position�е������ᱻ��ӵ�ͼԪ������
��EndPrimitive������ʱ�����з������(Emitted)���㶼��ϳ�Ϊָ���������ȾͼԪ��������triangle_strip��
��һ������EmitVertex����֮���ظ�����EndPrimitive�ܹ����ɶ��ͼԪ��
����������У����Ƿ�����5�����㣬(max_vertices = 5)�����Ǵ�ԭʼ����λ��ƽ����һ�ξ��룬
֮�������EndPrimitive��ֻҪ˳����ȷ��ÿ����������ͻ��һ�������Ρ�
*/