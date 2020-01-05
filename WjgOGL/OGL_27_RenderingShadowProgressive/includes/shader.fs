#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

void main()
{      
	//如果我们不（启用GL_FACE_CULL来）使用面剔除，
	//那么gl_FrontFacing将会告诉我们当前片段是属于正向面的一部分还是背向面的一部分  
	//gl_FrontFacing变量是一个bool，如果当前片段是正向面的一部分那么就是true，
	//否则就是false。比如说，我们可以这样子创建一个立方体，
	//在内部和外部使用不同的纹理     
    if(gl_FrontFacing)
        FragColor = texture(frontTexture, TexCoords);
    else
        FragColor = texture(backTexture, TexCoords);
}