#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;

//�Զ����ƫ������������Ƭ����Χ��8������Ƭ��
const float offset = 1.0 / 300.0;  

//Ƭ����ɫ������ӻ�������������Ψһһ���¾��Ǵ����ɵ������в���
void main()
{    
	//	9������Ƭ�ε�λ��
	vec2 offsets[9] = vec2[](
        vec2(-offset,  offset), // ����
        vec2( 0.0f,    offset), // ����
        vec2( offset,  offset), // ����
        vec2(-offset,  0.0f),   // ��
        vec2( 0.0f,    0.0f),   // ��
        vec2( offset,  0.0f),   // ��
        vec2(-offset, -offset), // ����
        vec2( 0.0f,   -offset), // ����
        vec2( offset, -offset)  // ����
    );

	//������������Ч����һ����Ե���(Edge Detection)��Ч��
	float kernel[9] = float[](
		1, 1, 1,
		1,-8, 1,
		1, 1, 1
	);

	//�������������Ƭ���Լ��Լ���Χ��8������Ƭ��
    vec3 sampleTex[9];
    for(int i = 0; i < 9; i++)
    {
        sampleTex[i] = vec3(texture(screenTexture, TexCoords.st + offsets[i]));
    }

	//�������ĺ���˼����ǣ���Ȩ����
    vec3 col = vec3(0.0);
    for(int i = 0; i < 9; i++)
	{
        col += sampleTex[i] * kernel[i];
	}

    FragColor = vec4(col, 1.0);
}