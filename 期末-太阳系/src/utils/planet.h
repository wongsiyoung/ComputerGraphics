#ifndef PLANET_H
#define PLANET_H
#include "vec3.h"
#include "texture.h"

class Planet{
public:
  float revolution;       // 公转
  float addRevolution;    // 公转每天增加的角度
  float rotation;         // 自转
  float addRotation;      // 自转每天增加的角度
  float radius;           // 星球的半径
  float slope;            // 转轴倾斜角度
  
  vec3 pos;               //下面这个变量代表当前行星离父亲之间的的距离
 
  Texture *texture;       // 星球绑定的纹理类

  // 材质属性
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