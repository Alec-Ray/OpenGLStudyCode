#version 330 core
in vec4 FragPos;

uniform vec3 lightPos;
uniform float far_plane;

void main()
{
	//这个深度就是每个fragment位置和光源位置之间的线性距离
    float lightDistance = length(FragPos.xyz - lightPos);
    lightDistance = lightDistance / far_plane;
    
    gl_FragDepth = lightDistance;
}