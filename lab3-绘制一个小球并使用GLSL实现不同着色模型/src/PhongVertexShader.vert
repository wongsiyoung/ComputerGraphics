/*###################################################
##  顶点着色器
##  描述： 根据顶点位置和法向量,输出顶点的颜色和法向量以及位置的变换,用于着色器计算漫反射
#####################################################*/
#version 410 core
layout (location = 0) in vec3 aPos;         // 顶点位置
layout (location = 1) in vec3 normal;       // 顶点法向量

out vec3 vertColor;                         // 顶点颜色
out vec3 FragNormal;                        // 法向量经过 model 变换后值
out vec3 FragPos;                           // 在世界坐标系中的位置
out vec4 lightPos;                          // 光在世界中的位置

uniform mat4 projection;                    // PROJECTIN 矩阵
uniform mat4 model;                         // MODEL 矩阵
uniform mat4 view;                          // VIEW 矩阵
uniform float twisting;

void main(){
     //twisting
    float Angle = twisting * length(aPos.xy);
    float s = sin(Angle);
    float c = cos(Angle);
    vec3 Pos;
    Pos.x = c * aPos.x - s * aPos.y;
    Pos.y = s * aPos.x + c * aPos.y;
    Pos.z = aPos.z;
    gl_Position = projection * view * model * vec4(Pos, 1.0);

    // gl_Position = projection * view * model * vec4(aPos, 1.0);              // 设置顶点位置
    vertColor = (vec3(gl_Position.xyz) + vec3(0.1f,0.1f,0.1f)) * 0.5f;      // 设置顶点颜色
    mat3 normalMatrix = mat3(transpose(inverse(model)));                    
    FragNormal = normalMatrix * normal;                                     // 计算法向量经过模型变换后值
    FragPos = vec3(model * vec4(aPos, 1.0));                                // 计算顶点在世界坐标系中的位置
    lightPos = view * model * vec4(-2.0f, -2.0f, 0.0f,1.0f);       	            // 光的位置
}