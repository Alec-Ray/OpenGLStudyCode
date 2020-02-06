#version 330 core
out float FragColor;

//这个着色器将对应G缓冲纹理(包括线性深度)，噪声纹理和法向半球核心样本作为输入参数

in vec2 TexCoords;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D texNoise;

uniform vec3 samples[64];

// 64个核心样本，半径，一个小的偏移量(这几个参数都是可以调节的)
int kernelSize = 64;
float radius = 0.5;
float bias = 0.025;

//noiseScale的变量。我们想要将噪声纹理平铺(Tile)在屏幕上，但是由于TexCoords的取值在0.0和1.0之间，
//texNoise纹理将不会平铺。所以我们将通过屏幕分辨率除以噪声纹理大小的方式计算TexCoords的缩放大小，
//并在之后提取相关输入向量的时候使用。
// 屏幕的平铺噪声纹理会根据屏幕分辨率除以噪声大小的值来决定
const vec2 noiseScale = vec2(1000.0/4.0, 800.0/4.0); 

uniform mat4 projection;

void main()
{
    // 获取SSAO算法需要的数据
    vec3 fragPos = texture(gPosition, TexCoords).xyz;
    vec3 normal = normalize(texture(gNormal, TexCoords).rgb);
    vec3 randomVec = normalize(texture(texNoise, TexCoords * noiseScale).xyz);
    // 一个TBN矩阵，将向量从切线空间变换到观察空间
    vec3 tangent = normalize(randomVec - normal * dot(randomVec, normal));
    vec3 bitangent = cross(normal, tangent);
    mat3 TBN = mat3(tangent, bitangent, normal);
    // 迭代样本内核并计算遮挡因子
    float occlusion = 0.0;
    for(int i = 0; i < kernelSize; ++i)
    {
        // 获取样本位置
		// 切线->观察空间
		//对于每一次迭代我们首先变换各自样本到观察空间。
		//之后我们会加观察空间核心偏移样本到观察空间片段位置上；
		//最后再用radius乘上偏移样本来增加(或减少)SSAO的有效取样半径。
        vec3 sample = TBN * samples[i]; 
        sample = fragPos + sample * radius; 
        
        vec4 offset = vec4(sample, 1.0);
		//由于这个向量目前在观察空间，我们将首先使用projection矩阵uniform变换它到裁剪空间
        offset = projection * offset; 
		// 透视划分
        offset.xyz /= offset.w;
		// 变换到0.0 - 1.0的值域
        offset.xyz = offset.xyz * 0.5 + 0.5; 
        
        // 使用offset向量的x和y分量采样线性深度纹理从而获取样本位置从观察者视角的深度值
        float sampleDepth = texture(gPosition, offset.xy).z; 
        
		//引入一个范围测试从而保证我们只当被测深度值在取样半径内时影响遮蔽因子
        //检查样本的当前深度值是否大于存储的深度值，如果是的，添加到最终的贡献因子上
        float rangeCheck = smoothstep(0.0, 1.0, radius / abs(fragPos.z - sampleDepth));
        occlusion += (sampleDepth >= sample.z + bias ? 1.0 : 0.0) * rangeCheck;           
    }
	//用1.0减去了遮蔽因子，以便直接使用遮蔽因子去缩放环境光照分量
    occlusion = 1.0 - (occlusion / kernelSize);
    
    FragColor = occlusion;
}