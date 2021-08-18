#ifndef CAMERA_H
#define CAMERA_H
#include "base.h"
#include <cmath>
#include "vec3.h"

/*###################################################
##  �ࣺCamera
##  �����������ӽ�
#####################################################*/
class Camera{
public :  
    vec3  pos;              //�����������ռ�����
    vec3  forward;          // ǰ��
    vec3  right;            // ����
    vec3  up;               // ��������Ϸ���
    float pitch_speed;      //�����Ǳ任�ٶ�
    float yaw_speed;        //ƫ�����ƶ��ٶ�
    float pitch, yaw;       //�����ǣ�ƫ����ƫ�ƽǶ�

    Camera() {}
    Camera(vec3 at)
    {
      pos=at;
      pitch_speed = yaw_speed = 0.05f;
      pitch = 0.0f;
      yaw = 90.0f;
      forward = vec3(0.0f,0.0f,-1.0f);  // ǰ��ƫ����
      right = vec3(1.0f,0.0f,0.0f);     // ����ƫ����
      up = vec3(0.0f,1.0f,0.0f);        // ���Ϸ���
    }

/*###################################################
##  ����: update
##  ���������� MODELVIEW ����
##  ���������� ��
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
##  ����: ProcessMouseMovement
##  ���������� ����������ӽ��ƶ�
##  ���������� xOffset ˮƽ�����ƶ�����
##           yOffset ��ֱ�����ƶ�����
#####################################################*/
void processMouseMovement(float xOffset, float yOffset)
{
    //�����ӽ�ƫ����
    xOffset *= yaw_speed;
    yOffset *= pitch_speed;
    //���¸����ǣ�ƫ���ǽǶ�
    yaw += xOffset;
    pitch += yOffset;

    if (pitch > 89.0f) pitch = 89.0f;
    if (pitch < -89.0f) pitch = -89.0f;
    //�������������ϵ
    vec3 tmp;
    tmp.x = std::cos(radian(yaw)) * std::cos(radian(pitch));
    tmp.y = std::sin(radian(pitch));
    tmp.z = std::sin(radian(yaw)) * std::cos(radian(pitch));
    forward = normalize(tmp);
    right = normalize(cross(forward, up));
    // up = normalize(cross(right, forward));

}
/*###################################################
##  ����: radian
##  ���������� �Ƕ�ת����
##  ���������� angle �Ƕ�
#####################################################*/
float radian(const float angle)
{
    return angle * 3.1415926535897626f / 180.0f;
}
      
};
#endif