#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;

//自定义的偏移量，采样此片段周围的8个像素片段
const float offset = 1.0 / 300.0;  

//片段着色器会更加基础，我们做的唯一一件事就是从生成的纹理中采样
void main()
{    
	//	9个像素片段的位置
	vec2 offsets[9] = vec2[](
        vec2(-offset,  offset), // 左上
        vec2( 0.0f,    offset), // 正上
        vec2( offset,  offset), // 右上
        vec2(-offset,  0.0f),   // 左
        vec2( 0.0f,    0.0f),   // 中
        vec2( offset,  0.0f),   // 右
        vec2(-offset, -offset), // 左下
        vec2( 0.0f,   -offset), // 正下
        vec2( offset, -offset)  // 右下
    );

	//卷积矩阵，这里的效果是一个边缘检测(Edge Detection)的效果
	float kernel[9] = float[](
		1, 1, 1,
		1,-8, 1,
		1, 1, 1
	);

	//采样自身的像素片段以及自己周围的8个像素片段
    vec3 sampleTex[9];
    for(int i = 0; i < 9; i++)
    {
        sampleTex[i] = vec3(texture(screenTexture, TexCoords.st + offsets[i]));
    }

	//卷积运算的核心思想就是：加权叠加
    vec3 col = vec3(0.0);
    for(int i = 0; i < 9; i++)
	{
        col += sampleTex[i] * kernel[i];
	}

    FragColor = vec4(col, 1.0);
}