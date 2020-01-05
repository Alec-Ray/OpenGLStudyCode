#version 330 core

void main()
{         
	//这个片元着色器不需要处理颜色缓冲
	//显示设置深度缓冲，也可以注释掉下面这句话，效率会更高，因为底层会自动设置深度缓冲
    gl_FragDepth = gl_FragCoord.z;
}