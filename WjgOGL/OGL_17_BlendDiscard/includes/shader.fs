#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D texture1;

void main()
{   
	vec4 texColor = texture(texture1, TexCoords);
	//GLSL��discardָ�ɾ����Ƭ��
	if (texColor.a < 0.1)
		discard;
    FragColor = texColor;
}