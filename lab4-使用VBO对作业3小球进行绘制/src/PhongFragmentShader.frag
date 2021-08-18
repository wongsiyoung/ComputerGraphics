/*###################################################
##  片元着色器
##  描述： 根据物体颜色、物体位置、顶点法向量计算 phong shading
#####################################################*/
#version 410 core
in vec3 vertColor;      // 物体颜色
in vec3 FragPos;        // 物体在世界坐标的位置
in vec3 FragNormal;     // 法向量
in vec4 lightPos;       // 光的位置
out vec4 FragColor;     // 最终渲染的颜色

//vec3 lightColor = vec3(1.0f, 1.0f, 1.0f);       // 光的强度
vec3 lightColor = vec3(0.5f, 0.5f, 0.5f);  // 光的强度
vec3 viewPos = vec3(0.0f,0.0f,0.0f);            // 观察位置

void main(){
    // 环境光
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor * vertColor;

    // 漫反射光成分
    vec3 lightDir = normalize(lightPos.xyz - FragPos);
    vec3 normal = normalize(FragNormal);
    float diffFactor = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = diffFactor * lightColor * vertColor;

    // 镜面反射成分
    float   specularStrength = 1.0f;
    vec3    reflectDir = normalize(reflect(-lightDir, normal));
    vec3    viewDir = normalize(viewPos - FragPos);
    float   specFactor = pow(max(dot(reflectDir, viewDir), 0.0), 32);           // 最后一个参数为镜面高光系数
    vec3    specular = specularStrength * specFactor * lightColor * vertColor;    

    vec3 result = ambient + diffuse + specular; 
    FragColor = vec4(result,1.0f);
}
