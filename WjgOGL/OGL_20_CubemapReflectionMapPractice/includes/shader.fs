#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 Position;

uniform vec3 cameraPos;
//samplerCube��������ͼ
uniform samplerCube skybox;

in vec2 TexCoords;
uniform sampler2D texture_reflection1;
uniform sampler2D texture_diffuse1;

//�����Ӻͽ�ɫ���з���
void main()
{    
	//viewdir
    vec3 I = normalize(Position - cameraPos);

    vec3 R = reflect(I, normalize(Normal));
	vec3 reflectionMap = vec3(texture(texture_reflection1,TexCoords)); 
	//���������պе���ɫ�ͷ�����ͼ����ɫ����
	vec3 reflection = vec3(texture(skybox, R).rgb) * reflectionMap;

	float diff = max(normalize(dot(normalize(Normal), -I)), 0.0f);
	vec3 diffuse = diff * vec3(texture(texture_diffuse1, TexCoords));

    FragColor = vec4(diffuse + reflection, 1.0f);
}