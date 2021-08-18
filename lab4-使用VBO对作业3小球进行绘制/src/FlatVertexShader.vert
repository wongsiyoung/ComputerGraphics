/*###################################################
##  顶点着色器
##  描述： 根据顶点颜色、顶点法向量计算 ground shading
#####################################################*/
#version 450 compatibility
layout (location = 0) in vec3 position;		// 顶点位置
layout (location = 1) in vec3 normal;		// 顶点法向量

flat out vec3 vertColor;					// 计算后的顶点颜色

uniform mat4 projection;					// PROJECTION 矩阵
uniform mat4 model;							// MODEL 矩阵
uniform mat4 view;							// VIEW 矩阵
uniform int mode;							// 绘制方式

//vec3 lightColor = vec3(1.0f, 1.0f, 1.0f);  		// 光的强度
vec3 lightColor = vec3(0.5f, 0.5f, 0.5f);	// 光的强度
vec3 lightPos = vec3(-2.0f, -2.0f, 0.0f);   // 光的位置
vec3 viewPos = vec3(0.0f,0.0f,0.0f);			// 观察位置

void main(){

    vec3 NORMAL;
    if(mode == 0){				// vbo 绘制
        NORMAL = normal;
    }else{						// 另外两种方式绘制
        NORMAL = gl_Normal;
    }

	gl_Position = projection * view * model * vec4(position, 1.0);				// 设置顶点位置
	vec3 objectColor = (vec3(gl_Position.xyz) + vec3(1.0f,1.0f,1.0f)) * 0.5f;	// 计算物体颜色
	vec3 Position = vec3(model * vec4(position, 1.0)); 							// 在世界坐标系中指定片元位置
	mat3 normalMatrix = mat3(transpose(inverse(model)));
	vec3 Normal = normalize(normalMatrix * NORMAL); 							// 计算法向量经过模型变换后值

	// 环境光成分
	float	ambientStrength = 0.1f;
	vec3	ambient = ambientStrength * lightColor;

	// 漫反射光成分
	vec4 light = view * model * vec4(lightPos,1.0f);
	vec3	lightDir = normalize(light.xyz - Position);
	float	diffFactor = max(dot(lightDir, Normal), 0.0);
	vec3	diffuse = diffFactor * lightColor;

	// 镜面反射成分
	float	specularStrength = 1.0f;
	vec3	reflectDir = normalize(reflect(-lightDir, Normal));
	vec3	viewDir = normalize(viewPos - Position);
	float	specFactor = pow(max(dot(reflectDir, viewDir), 0.0), 32); 			// 最后一个参数为镜面高光系数
	vec3	specular = specularStrength * specFactor * lightColor;

	vec3 result = ambient + diffuse + specular;			
	vertColor = result * objectColor;
}