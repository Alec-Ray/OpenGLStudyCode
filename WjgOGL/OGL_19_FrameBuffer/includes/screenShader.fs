#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;

//Ƭ����ɫ������ӻ�������������Ψһһ���¾��Ǵ����ɵ������в���
void main()
{    
    //FragColor = texture(texture1, TexCoords);
	FragColor = texture(screenTexture, TexCoords);
}