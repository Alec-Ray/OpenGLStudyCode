#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

void main()
{      
	//������ǲ�������GL_FACE_CULL����ʹ�����޳���
	//��ôgl_FrontFacing����������ǵ�ǰƬ���������������һ���ֻ��Ǳ������һ����  
	//gl_FrontFacing������һ��bool�������ǰƬ�����������һ������ô����true��
	//�������false������˵�����ǿ��������Ӵ���һ�������壬
	//���ڲ����ⲿʹ�ò�ͬ������     
    if(gl_FrontFacing)
        FragColor = texture(frontTexture, TexCoords);
    else
        FragColor = texture(backTexture, TexCoords);
}