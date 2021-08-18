/*###################################################
##  片元着色器
##  描述： 根据顶点顶点颜色输出片元颜色
#####################################################*/
#version 410 core
in vec3 vertColor;
out vec4 FragColor;

void main(){
    FragColor = vec4(vertColor ,1.0f);
}
