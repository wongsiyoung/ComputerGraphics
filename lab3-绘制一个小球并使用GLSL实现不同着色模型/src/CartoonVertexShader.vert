/*###################################################
##  顶点着色器
##  描述： 根据顶点位置和法向量,输出顶点的颜色和法向量以及位置的变换,用于片元着色器计算 Cartoon 模型
#####################################################*/
#version 410 core
layout (location = 0) in vec3 position;			// 顶点位置
layout (location = 1) in vec3 normal;			// 顶点法向量

out vec3 vertColor;								// 顶点颜色				
out vec3 vertNormal;							// 法向量经过模型变换后值
out vec3 vertLight;								// 光经过变换后的值

uniform mat4 projection;						// PROJECTION 矩阵
uniform mat4 model;								// MODEL 矩阵
uniform mat4 view;								// VIEW 矩阵

//vec3 lightColor = vec3(1.0f, 1.0f, 1.0f);  		// 光的强度
vec3 lightColor = vec3(0.5f, 0.5f, 0.5f);  	// 光的强度
vec3 lightPos = vec3(-2.0f, -2.0f, 0.0f);       // 光的位置

void main(){
	
	mat3 normalMatrix = mat3(transpose(inverse(model)));
	vertNormal = normalize(normalMatrix * normal);			// 计算法向量经过模型变换后值

	vec4 viewLight = vec4(lightPos, 1.0);
	vertLight = viewLight.xyz;								// 计算光经过坐标变换后的值

	gl_Position = projection * view * model * vec4(position, 1.0);			// 设置顶点位置
	vertColor = (vec3(gl_Position.xyz) + vec3(1.0f,1.0f,1.0f)) * 0.5f;		// 设置顶点颜色
}