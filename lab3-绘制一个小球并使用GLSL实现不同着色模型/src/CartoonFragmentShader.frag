/*###################################################
##  片元着色器
##  描述： 根据顶点颜色和法向量和光照,得到 cartoon 模型
#####################################################*/
#version 410 core
in vec3 vertColor;          // 顶点颜色
in vec3 vertNormal;         // 法向量经过模型变换后值
in vec3 vertLight;          // 光经过变换后的值
out vec4 FragColor;         // 片元颜色

void main(){
    float diffuse = dot(normalize(vertLight), vertNormal);
    if (diffuse > 0.8) {
        diffuse = 1.0;
    }
    else if (diffuse > 0.5) {
        diffuse = 0.6;
    }
    else if (diffuse > 0.2) {
        diffuse = 0.4;
    }
    else {
        diffuse = 0.2;
    }
    FragColor = vec4(vertColor * diffuse, 1.0);
}

/*void main(){
    float silhouette = length(vertNormal * vec3(0.0, 0.0, 1.0));
    if (silhouette < 0.5) {
        silhouette = 0.0;
    }
    else {
        silhouette = 1.0;
    }

    float diffuse = dot(normalize(vertLight), vertNormal);
    if (diffuse > 0.8) {
        diffuse = 1.0;
    }
    else if (diffuse > 0.5) {
        diffuse = 0.6;
    }
    else if (diffuse > 0.2) {
        diffuse = 0.4;
    }
    else {
        diffuse = 0.2;
    }
    diffuse = diffuse * silhouette;
    FragColor = vec4(vertColor * diffuse, 1.0);
}*/