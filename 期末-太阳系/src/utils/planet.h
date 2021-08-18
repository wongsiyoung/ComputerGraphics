#ifndef PLANET_H
#define PLANET_H
#include "vec3.h"
#include "texture.h"

class Planet{
public:
  float revolution;       // ��ת
  float addRevolution;    // ��תÿ�����ӵĽǶ�
  float rotation;         // ��ת
  float addRotation;      // ��תÿ�����ӵĽǶ�
  float radius;           // ����İ뾶
  float slope;            // ת����б�Ƕ�
  
  vec3 pos;               //���������������ǰ�����븸��֮��ĵľ���
 
  Texture *texture;       // ����󶨵�������

  // ��������
  float ambient[4];
  float diffuse[4];
  float specular[4];
  float emission[4];
  float shininess;

public:
 
  Planet(const char* texname,float revolution,float rotation,float slope,float radius,vec3 pos)
  {
    texture=new Texture(texname,true);
    this->slope = slope;
    this->revolution = 0.0f;
    this->addRevolution = revolution;
    this->rotation = 0.0f;
    this->addRotation = rotation;
    this->radius =radius;
    this->pos =pos;
   
  }

  ~Planet()
  {
	  if (!texture)
	  {
		  delete texture;
		  texture = NULL;
	  }
  }

};
#endif