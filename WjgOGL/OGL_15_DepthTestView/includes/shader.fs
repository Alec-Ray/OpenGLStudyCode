#version 330 core
out vec4 FragColor;

//线性深度缓冲
float near = 0.1; 
float far = 100.0; 
float LinearizeDepth(float depth) 
{
    float z = depth * 2.0 - 1.0; // back to NDC 
    return (2.0 * near * far) / (far + near - z * (far - near));	
}

void main()
{             
	//返回深度[0-1]
    float depth = LinearizeDepth(gl_FragCoord.z) / far; 

	//把深度值作为片段的颜色，离得越近越黑，越远越接近1也就越白
    FragColor = vec4(vec3(depth), 1.0);
}