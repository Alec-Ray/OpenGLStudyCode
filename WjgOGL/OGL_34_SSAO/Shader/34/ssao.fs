#version 330 core
out float FragColor;

//�����ɫ������ӦG��������(�����������)����������ͷ���������������Ϊ�������

in vec2 TexCoords;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D texNoise;

uniform vec3 samples[64];

// 64�������������뾶��һ��С��ƫ����(�⼸���������ǿ��Ե��ڵ�)
int kernelSize = 64;
float radius = 0.5;
float bias = 0.025;

//noiseScale�ı�����������Ҫ����������ƽ��(Tile)����Ļ�ϣ���������TexCoords��ȡֵ��0.0��1.0֮�䣬
//texNoise��������ƽ�̡��������ǽ�ͨ����Ļ�ֱ��ʳ������������С�ķ�ʽ����TexCoords�����Ŵ�С��
//����֮����ȡ�������������ʱ��ʹ�á�
// ��Ļ��ƽ����������������Ļ�ֱ��ʳ���������С��ֵ������
const vec2 noiseScale = vec2(1000.0/4.0, 800.0/4.0); 

uniform mat4 projection;

void main()
{
    // ��ȡSSAO�㷨��Ҫ������
    vec3 fragPos = texture(gPosition, TexCoords).xyz;
    vec3 normal = normalize(texture(gNormal, TexCoords).rgb);
    vec3 randomVec = normalize(texture(texNoise, TexCoords * noiseScale).xyz);
    // һ��TBN���󣬽����������߿ռ�任���۲�ռ�
    vec3 tangent = normalize(randomVec - normal * dot(randomVec, normal));
    vec3 bitangent = cross(normal, tangent);
    mat3 TBN = mat3(tangent, bitangent, normal);
    // ���������ں˲������ڵ�����
    float occlusion = 0.0;
    for(int i = 0; i < kernelSize; ++i)
    {
        // ��ȡ����λ��
		// ����->�۲�ռ�
		//����ÿһ�ε����������ȱ任�����������۲�ռ䡣
		//֮�����ǻ�ӹ۲�ռ����ƫ���������۲�ռ�Ƭ��λ���ϣ�
		//�������radius����ƫ������������(�����)SSAO����Чȡ���뾶��
        vec3 sample = TBN * samples[i]; 
        sample = fragPos + sample * radius; 
        
        vec4 offset = vec4(sample, 1.0);
		//�����������Ŀǰ�ڹ۲�ռ䣬���ǽ�����ʹ��projection����uniform�任�����ü��ռ�
        offset = projection * offset; 
		// ͸�ӻ���
        offset.xyz /= offset.w;
		// �任��0.0 - 1.0��ֵ��
        offset.xyz = offset.xyz * 0.5 + 0.5; 
        
        // ʹ��offset������x��y�������������������Ӷ���ȡ����λ�ôӹ۲����ӽǵ����ֵ
        float sampleDepth = texture(gPosition, offset.xy).z; 
        
		//����һ����Χ���ԴӶ���֤����ֻ���������ֵ��ȡ���뾶��ʱӰ���ڱ�����
        //��������ĵ�ǰ���ֵ�Ƿ���ڴ洢�����ֵ������ǵģ���ӵ����յĹ���������
        float rangeCheck = smoothstep(0.0, 1.0, radius / abs(fragPos.z - sampleDepth));
        occlusion += (sampleDepth >= sample.z + bias ? 1.0 : 0.0) * rangeCheck;           
    }
	//��1.0��ȥ���ڱ����ӣ��Ա�ֱ��ʹ���ڱ�����ȥ���Ż������շ���
    occlusion = 1.0 - (occlusion / kernelSize);
    
    FragColor = occlusion;
}