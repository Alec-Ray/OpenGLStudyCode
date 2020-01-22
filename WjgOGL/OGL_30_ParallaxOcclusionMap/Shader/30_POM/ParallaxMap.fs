#version 330 core
out vec4 FragColor;

in VS_OUT {
    vec3 FragPos;
    vec2 TexCoords;
    vec3 TangentLightPos;
    vec3 TangentViewPos;
    vec3 TangentFragPos;
} fs_in;

uniform sampler2D diffuseMap;
uniform sampler2D normalMap;
uniform sampler2D depthMap;

uniform float heightScale;

//视差遮蔽映射的偏移纹理的算法
//ParallaxMapping的函数，它把fragment的纹理坐标作和切线空间中的fragment到观察者的方向向量为输入。
//这个函数返回经位移的纹理坐标。然后我们使用这些经位移的纹理坐标进行diffuse和法线贴图的采样
vec2 ParallaxMapping(vec2 texCoords, vec3 viewDir)
{ 
    // 首先定义层的数量，在垂直看时使用更少的样本，以一定角度看时增加样本数量
    const float minLayers = 8;
    const float maxLayers = 32;
    float numLayers = mix(maxLayers, minLayers, abs(dot(vec3(0.0, 0.0, 1.0), viewDir)));
	  
    // 计算每一层的深度
    float layerDepth = 1.0 / numLayers;

    // 当前层的深度
    float currentLayerDepth = 0.0;

    // 每一层沿着P¯的方向进行移动
    vec2 P = viewDir.xy / viewDir.z * heightScale; 
    vec2 deltaTexCoords = P / numLayers;
  
    // 赋初值
    vec2  currentTexCoords     = texCoords;
    float currentDepthMapValue = texture(depthMap, currentTexCoords).r;
     
	// 遍历所有层，从上开始，直到找到小于这一层的深度值的深度贴图值
    while(currentLayerDepth < currentDepthMapValue)
    {
        // 纹理坐标递减
        currentTexCoords -= deltaTexCoords;
        // 从高度贴图里得到高度值
        currentDepthMapValue = texture(depthMap, currentTexCoords).r;  
        // 获取下一层的高度值
        currentLayerDepth += layerDepth;  
    }

	//视差遮蔽映射(Parallax Occlusion Mapping)和陡峭视差映射的原则相同，但不是用触碰的第一个深度层的纹理坐标，而是在触碰之前和之后，在深度层之间进行线性插值

	//视差遮蔽映射的代码基于陡峭视差映射，以下是视察遮蔽映射增加的代码：
	// 获取交叉前的纹理坐标
    vec2 prevTexCoords = currentTexCoords + deltaTexCoords;

    // 计算来自相应深度层的几何之间的深度之间的距离
    float afterDepth  = currentDepthMapValue - currentLayerDepth;
    float beforeDepth = texture(depthMap, prevTexCoords).r - currentLayerDepth + layerDepth;
 
    // 纹理插值，计算下两个纹理的权重
    float weight = afterDepth / (afterDepth - beforeDepth);
    vec2 finalTexCoords = prevTexCoords * weight + currentTexCoords * (1.0 - weight);
    
    return finalTexCoords;
}

void main()
{           
    // 通过视差贴图计算得出偏移后的纹理坐标
    vec3 viewDir = normalize(fs_in.TangentViewPos - fs_in.TangentFragPos);
    vec2 texCoords = fs_in.TexCoords;
    texCoords = ParallaxMapping(fs_in.TexCoords,  viewDir);     
	  
	//超出默认纹理坐标范围进行采样的时候就丢弃这个fragment
    if(texCoords.x > 1.0 || texCoords.y > 1.0 || texCoords.x < 0.0 || texCoords.y < 0.0)
        discard;

    // obtain normal from normal map
    vec3 normal = texture(normalMap, texCoords).rgb;
    normal = normalize(normal * 2.0 - 1.0);   
   
    vec3 color = texture(diffuseMap, texCoords).rgb;
    vec3 ambient = 0.1 * color;
    vec3 lightDir = normalize(fs_in.TangentLightPos - fs_in.TangentFragPos);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * color;
    vec3 reflectDir = reflect(-lightDir, normal);
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    float spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);

    vec3 specular = vec3(0.2) * spec;
    FragColor = vec4(ambient + diffuse + specular, 1.0);
}