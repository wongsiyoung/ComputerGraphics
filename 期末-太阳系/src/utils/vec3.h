#ifndef __BMATH_H
#define __BMATH_H
#include "math.h"

#define EPSILON 1e-6f
#define PI 3.14159265358979323846f
#define DEG2RAD (PI / 180.0f)
#define RAD2DEG (180.0f / PI)
/*###################################################
##  结构体：vec3
##  描述：三维向量
#####################################################*/
struct vec3 {
	// float x, y, z;
	union {
		struct {
			float x,y,z;
		};
		float v[3];
	};
	/*###################################################
	##  函数：构造函数
	##  函数描述：分别实现默认构造、通过三个float值构造、通过float数组构造、通过另一个vec3构造
	##  参数描述：默认构造函数无参数；
	##			 float x、y、z：对应vec3的x、y、z；
	##			 float *v：前三个数对应vec3的x、y、z；
	##			 vec3 &v：v的x、y、z对应新的vec3的x、y、z；
	#####################################################*/
	inline vec3() : x(0), y(0), z(0) {}
	inline vec3(float x,float y,float z) : x(x), y(y), z(z) {}
	inline vec3(const float *v) : x(v[0]), y(v[1]), z(v[2]) {}
	inline vec3(const vec3 &v) : x(v.x), y(v.y), z(v.z) {}
	
	inline int operator==(const vec3 &v);
	inline int operator!=(const vec3 &v);
	inline const vec3 operator*(float f) const;
	inline const vec3 operator/(float f) const;
	inline const vec3 operator+(const vec3 &v) const;
	inline const vec3 operator-() const;
	inline const vec3 operator-(const vec3 &v) const;
	inline vec3 &operator*=(float f);
	inline vec3 &operator/=(float f);
	inline vec3 &operator+=(const vec3 &v);
	inline vec3 &operator-=(const vec3 &v);
	inline float operator*(const vec3 &v) const;
	inline operator float*();
	inline operator const float*() const;
	inline float &operator[](int i);
	inline const float operator[](int i) const;
	inline float length() const;
	inline float normalize();
	inline vec3 normalize(const vec3& v);
	inline void cross(const vec3 &v1,const vec3 &v2);
};
/*###################################################
##  函数: operator==
##  函数描述： 重载运算符==，判断两个vec3是否相等
##  参数描述： v：vec3向量
#####################################################*/
inline int vec3::operator==(const vec3 &v) 
{ 
	return (fabs(x - v.x) < EPSILON && fabs(y - v.y) < EPSILON && fabs(z - v.z) < EPSILON); 
}
/*###################################################
##  函数: operator!=
##  函数描述： 重载运算符!=，判断两个vec3是否不相等
##  参数描述： v：vec3向量
#####################################################*/
inline int vec3::operator!=(const vec3 &v) 
{
	return !(*this == v); 
}
/*###################################################
##  函数: operator*
##  函数描述： 重载运算符*，实现向量和浮点数的乘法
##  参数描述： f：与向量相乘的浮点数
#####################################################*/
inline const vec3 vec3::operator*(float f) const 
{ 
	return vec3(x * f,y * f,z * f); 
}
/*###################################################
##  函数: operator/
##  函数描述： 重载运算符/，实现向量和浮点数的除法
##  参数描述： f：与向量相除的浮点数
#####################################################*/
inline const vec3 vec3::operator/(float f) const 
{
	return vec3(x / f,y / f,z / f); 
}
/*###################################################
##  函数: operator+
##  函数描述： 重载运算符+，实现向量和向量的加法
##  参数描述： v：vec3向量
#####################################################*/
inline const vec3 vec3::operator+(const vec3 &v) const 
{ 
	return vec3(x + v.x,y + v.y,z + v.z); 
}
/*###################################################
##  函数: operator-
##  函数描述： 重载运算符-，实现向量的取反
##  参数描述： 无
#####################################################*/
inline const vec3 vec3::operator-() const 
{ 
	return vec3(-x,-y,-z); 
}
/*###################################################
##  函数: operator-
##  函数描述： 重载运算符-，实现向量和向量的减法
##  参数描述： v：vec3向量
#####################################################*/
inline const vec3 vec3::operator-(const vec3 &v) const 
{ 
	return vec3(x - v.x,y - v.y,z - v.z); 
}
/*###################################################
##  函数: operator*=
##  函数描述： 重载运算符*=，实现向量和浮点数的乘法
##  参数描述： f：与向量相乘的浮点数
#####################################################*/
inline vec3 & vec3::operator*=(float f) 
{ 
	return *this = *this * f; 
}
/*###################################################
##  函数: operator/=
##  函数描述： 重载运算符/=，实现向量和浮点数的除法
##  参数描述： f：与向量相除的浮点数
#####################################################*/
inline vec3 & vec3::operator/=(float f) 
{ 
	return *this = *this / f; 
}
/*###################################################
##  函数: operator+=
##  函数描述： 重载运算符+=，实现向量和向量的加法
##  参数描述： v：vec3向量
#####################################################*/
inline vec3 & vec3::operator+=(const vec3 &v) 
{ 
	return *this = *this + v; 
}
/*###################################################
##  函数: operator-=
##  函数描述： 重载运算符-=，实现向量和向量的减法
##  参数描述： v：vec3向量
#####################################################*/
inline vec3 & vec3::operator-=(const vec3 &v) 
{ 
	return *this = *this - v; 
}
/*###################################################
##  函数: operator*
##  函数描述： 重载运算符*，实现向量和向量的点积
##  参数描述： v：vec3向量
#####################################################*/
inline float vec3::operator*(const vec3 &v) const 
{ 	
	return x * v.x + y * v.y + z * v.z; 
}
/*###################################################
##  函数: operator float*() 
##  函数描述： 重载运算符float*()，实现向量转换为浮点数
##  参数描述： 无
#####################################################*/
inline vec3::operator float*() 
{ 
	return (float*)&x; 
}
/*###################################################
##  函数: operator const float*() 
##  函数描述： 重载运算符const float*()，实现向量转换为浮点数
##  参数描述： 无
#####################################################*/
inline vec3::operator const float*() const 
{ 
	return (float*)&x; 
}
/*###################################################
##  函数: operator[]
##  函数描述： 重载运算符[]，实现读取向量三维数值
##  参数描述： i：取值为0-2，分别读取向量的x、y、z
#####################################################*/
inline float & vec3::operator[](int i) 
{ 
	return ((float*)&x)[i]; 
}
/*###################################################
##  函数: operator[]
##  函数描述： 重载运算符[]，实现读取向量三维数值
##  参数描述： i：取值为0-2，分别读取向量的x、y、z
#####################################################*/
inline const float vec3::operator[](int i) const 
{ 
	return ((float*)&x)[i]; 
}
/*###################################################
##  函数: length
##  函数描述： 读取向量的长度，定义为sqrt(x*x+y*y+z*z)
##  参数描述： 无
#####################################################*/
inline float vec3::length() const 
{ 
	return sqrt(x * x + y * y + z * z); 
}
/*###################################################
##  函数: normalize
##  函数描述： 将向量归一化，使得x*x+y*y+z*z = 1，返回向量原本的长度
##  参数描述： 无
#####################################################*/
inline float vec3::normalize() 
{
	float inv,length = sqrt(x * x + y * y + z * z);
	if(length < EPSILON) return 0.0;
	inv = 1.0f / length;
	x *= inv;
	y *= inv;
	z *= inv;
	return length;
	/* 理论上等价，可以改成下面这样~
	float length = sqrt(x * x + y * y + z * z);
	if(length < EPSILON) return 0.0;
	x /= length;
	y /= length;
	z /= length;
	return length;
	*/
}
/*###################################################
##  函数: normalize
##  函数描述： 向量归一化
##  参数描述： v 操作向量
#####################################################*/
inline vec3 normalize(const vec3& v)
{
    float invlen = 1.0f / v.length();
    return vec3(v.x*invlen, v.y*invlen, v.z*invlen);
}
/*###################################################
##  函数: cross
##  函数描述： 计算两个向量的叉积，即计算出一个向量，同时与两个向量垂直
##  参数描述： v1，v2：两个向量
#####################################################*/
inline void vec3::cross(const vec3 &v1,const vec3 &v2) 
{
	x = v1.y * v2.z - v1.z * v2.y;
	y = v1.z * v2.x - v1.x * v2.z;
	z = v1.x * v2.y - v1.y * v2.x;
}
/*###################################################
##  函数: cross
##  函数描述： 向量外积
##  参数描述： v1 v2 操作向量
#####################################################*/
inline vec3 cross(const vec3 &v1, const vec3& v2)
{
    return vec3(
         v1.y*v2.z - v2.y*v1.z,
         v1.z*v2.x - v1.x*v2.z,
         v1.x*v2.y - v1.y*v2.x
                );
}
#endif