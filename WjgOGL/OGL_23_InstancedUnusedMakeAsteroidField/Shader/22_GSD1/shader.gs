#version 330 core
layout (points) in;		//�������η�
layout (line_strip, max_vertices = 2) out;		//������η�

void main() {    
    gl_Position = gl_in[0].gl_Position + vec4(-0.1, 0.0, 0.0, 0.0); 
    EmitVertex();

    gl_Position = gl_in[0].gl_Position + vec4( 0.1, 0.0, 0.0, 0.0);
    EmitVertex();

    EndPrimitive();
}

//gl_in��OGL��һ�����ñ���
/*
ÿ�����ǵ���EmitVertexʱ��gl_Position�е������ᱻ��ӵ�ͼԪ������
��EndPrimitive������ʱ�����з������(Emitted)���㶼��ϳ�Ϊָ���������ȾͼԪ��������line_strip��
��һ������EmitVertex����֮���ظ�����EndPrimitive�ܹ����ɶ��ͼԪ��
����������У����Ƿ������������㣬(max_vertices = 2)�����Ǵ�ԭʼ����λ��ƽ����һ�ξ��룬
֮�������EndPrimitive��������������ϳ�Ϊһ���������������������
*/