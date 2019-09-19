#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;

//Ƭ����ɫ������ӻ�������������Ψһһ���¾��Ǵ����ɵ������в���
void main()
{    
	//FragColor = texture(screenTexture, TexCoords);

	//�Ƴ������г��˺ڰ׻��������е���ɫ��������ͼ��ҶȻ�(Grayscale)��
	//���ۻ����ɫ��������һЩ��������ɫ����ô���У�������ɫ��Ȩ�ظߡ������ʱ����Ȩ�أ���ȫƽ����������ɫ����Ҳ�е�ͨ
	FragColor = texture(screenTexture, TexCoords);
    float average = 0.2126 * FragColor.r + 0.7152 * FragColor.g + 0.0722 * FragColor.b;
	//ȡ���е���ɫ������������ƽ����
	//float average = (FragColor.r + FragColor.g + FragColor.b) / 3.0;
    FragColor = vec4(average, average, average, 1.0);
}