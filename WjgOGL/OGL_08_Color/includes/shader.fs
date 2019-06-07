#version 330 core
out vec4 FragColor;

uniform vec3 objectColor;	//物体本身的颜色
uniform vec3 lightColor;		//灯光颜色

void main()
{
	FragColor = vec4(lightColor * objectColor , 1.0f);
}