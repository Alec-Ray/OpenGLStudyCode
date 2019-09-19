#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;

//片段着色器会更加基础，我们做的唯一一件事就是从生成的纹理中采样
void main()
{    
	//FragColor = texture(screenTexture, TexCoords);

	//移除场景中除了黑白灰以外所有的颜色，让整个图像灰度化(Grayscale)。
	//人眼会对绿色更加敏感一些，而对蓝色不那么敏感，所以绿色的权重高。大多数时候不用权重，完全平均化三个颜色分量也行的通
	FragColor = texture(screenTexture, TexCoords);
    float average = 0.2126 * FragColor.r + 0.7152 * FragColor.g + 0.0722 * FragColor.b;
	//取所有的颜色分量，将它们平均化
	//float average = (FragColor.r + FragColor.g + FragColor.b) / 3.0;
    FragColor = vec4(average, average, average, 1.0);
}