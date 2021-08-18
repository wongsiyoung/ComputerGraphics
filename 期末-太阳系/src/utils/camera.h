#ifndef CAMERA_H
#define CAMERA_H
#include "base.h"
#include <cmath>
#include "vec3.h"

/*###################################################
##  类：Camera
##  描述：控制视角
#####################################################*/
class Camera{
public :  
    vec3  pos;              //摄像机的世界空间坐标
    vec3  forward;          // 前后
    vec3  right;            // 左右
    vec3  up;               // 摄像机向上方向
    float pitch_speed;      //俯仰角变换速度
    float yaw_speed;        //偏航角移动速度
    float pitch, yaw;       //俯仰角，偏航角偏移角度

    Camera() {}
    Camera(vec3 at)
    {
      pos=at;
      pitch_speed = yaw_speed = 0.05f;
      pitch = 0.0f;
      yaw = 90.0f;
      forward = vec3(0.0f,0.0f,-1.0f);  // 前后偏移量
      right = vec3(1.0f,0.0f,0.0f);     // 左右偏移量
      up = vec3(0.0f,1.0f,0.0f);        // 向上方向
    }

/*###################################################
##  函数: update
##  函数描述： MODELVIEW 设置
##  参数描述： 无
#####################################################*/
void update()
{
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    vec3 target = pos + forward;
    gluLookAt( 
      pos.x,pos.y,pos.z,
      target.x,target.y,target.z,
      up.x,up.y,up.z
    );
//  printf("lookat:\n");
//  printf("pos:%f,%f,%f",pos.x,pos.y,pos.z);
//  printf("target:%f,%f,%f",target.x,target.y,target.z);
//  printf("up:%f,%f,%f",up.x,up.y,up.z);
}

/*###################################################
##  函数: ProcessMouseMovement
##  函数描述： 控制摄像机视角移动
##  参数描述： xOffset 水平方向移动距离
##           yOffset 垂直方向移动距离
#####################################################*/
void processMouseMovement(float xOffset, float yOffset)
{
    //计算视角偏移量
    xOffset *= yaw_speed;
    yOffset *= pitch_speed;
    //更新俯仰角，偏航角角度
    yaw += xOffset;
    pitch += yOffset;

    if (pitch > 89.0f) pitch = 89.0f;
    if (pitch < -89.0f) pitch = -89.0f;
    //更新摄像机坐标系
    vec3 tmp;
    tmp.x = std::cos(radian(yaw)) * std::cos(radian(pitch));
    tmp.y = std::sin(radian(pitch));
    tmp.z = std::sin(radian(yaw)) * std::cos(radian(pitch));
    forward = normalize(tmp);
    right = normalize(cross(forward, up));
    // up = normalize(cross(right, forward));

}
/*###################################################
##  函数: radian
##  函数描述： 角度转弧度
##  参数描述： angle 角度
#####################################################*/
float radian(const float angle)
{
    return angle * 3.1415926535897626f / 180.0f;
}
      
};
#endif