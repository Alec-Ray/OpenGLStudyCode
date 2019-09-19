#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;

out vec2 TexCoords;

//֡�����������ɵ�ͼ��ĵĶ������ݣ���Щ��������Ļ�ռ��µ����ݣ���˲���Ҫ�ռ�任
//uniform mat4 model;
//uniform mat4 view;
//uniform mat4 projection;

void main()
{  
    //gl_Position = projection * view * model * vec4(aPos, 1.0);

	TexCoords = aTexCoords;  
	gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0); 
}

//Ϊ�˻�������ı��Σ����ǽ����´���һ�׼򵥵���ɫ����
//���ǽ���������κλ��ڵľ���任����Ϊ�����ṩ���Ǳ�׼���豸����Ķ������꣬
//�������ǿ���ֱ�ӽ������趨Ϊ������ɫ�������