#pragma once
#include<math.h>
#include<iostream>

typedef struct VECTOR3
{
public:
	VECTOR3() {};
	VECTOR3(float x, float y, float z);

	//重载运算符
	VECTOR3 operator + (VECTOR3 vec);
	VECTOR3 operator - (VECTOR3 vec);
	VECTOR3 operator * (float f);
	VECTOR3 operator / (float f);
	void    operator = (const VECTOR3 vec);

	float x = 0;
	float y = 0;
	float z = 0;
}VECTOR3;

typedef struct VECTOR4
{
public:
	VECTOR4() {};
	VECTOR4(float x, float y, float z, float w);

	//重载运算符
	VECTOR4 operator + (VECTOR4 vec);
	VECTOR4 operator - (VECTOR4 vec);
	VECTOR4 operator * (float f);
	VECTOR4 operator / (float f);
	void    operator = (const VECTOR4 vec);

	float x = 0;
	float y = 0;
	float z = 0;
	float w = 0;
}VECTOR4;

typedef struct MATRIX
{
public:
	MATRIX() {};
	MATRIX(float _11, float _12, float _13, float _14, 
		   float _21, float _22, float _23, float _24,
		   float _31, float _32, float _33, float _34,
		   float _41, float _42, float _43, float _44);
	MATRIX(VECTOR4 v1, VECTOR4 v2, VECTOR4 v3);

	//重载运算符
	MATRIX operator * (MATRIX);
	MATRIX operator + (MATRIX);
	MATRIX operator - (MATRIX);
	MATRIX operator * (float);
	MATRIX operator / (float);
	void   operator = (const MATRIX);

	float _11 = 0, _12 = 0, _13 = 0, _14 = 0;
	float _21 = 0, _22 = 0, _23 = 0, _24 = 0;
	float _31 = 0, _32 = 0, _33 = 0, _34 = 0;
	float _41 = 0, _42 = 0, _43 = 0, _44 = 0;
}MATRIX;

typedef struct COLOR
{
	COLOR() {};
	COLOR(int r, int g, int b, int w);
	COLOR operator + (COLOR);
	COLOR operator - (COLOR);
	COLOR operator * (float);
	COLOR operator / (float);
	void  operator = (const COLOR);
	bool  operator == (const COLOR);

	int r;
	int g;
	int b;
	int w;
}COLOR;

VECTOR3 Vec3Cross(VECTOR3* pv1, VECTOR3* pv2);
float Vec3Dot(VECTOR3* pv1, VECTOR3* pv2);
float Vec3Length(VECTOR3* pv);
VECTOR3 Vec3Normalize(VECTOR3* pv);

float Vec4Dot(VECTOR4* pv1, VECTOR4* pv2);
float Vec4Length(VECTOR4* pv);
VECTOR4 Vec4Transform(VECTOR4* pv, MATRIX* pm);
VECTOR4 Vec4Normalize(VECTOR4* pv);
VECTOR4 Vec4Cross(VECTOR4* pv1, VECTOR4* pv2);

MATRIX MatrixIdentity();
MATRIX MatrixRotationX(float Angle);
MATRIX MatrixRotationY(float Angle);
MATRIX MatrixRotationZ(float Angle);
MATRIX MatrixScalling(float sx, float sy, float sz);
MATRIX MatrixLookAtLH(VECTOR4* pEye, VECTOR4* pAt, VECTOR4* pUp);
MATRIX MatrixPerspectiveLH(float fovy, float Aspect, float zn, float zf);
VECTOR4 Wnormalization(VECTOR4* pv);

VECTOR3 Vec4toVec2d(VECTOR4 vec4);
float CalPlanePointZ(VECTOR4 v1, VECTOR4 v2, VECTOR4 v3, VECTOR4 vp);//已知三点坐标， 求面上另一点已知x，y的坐标的z
VECTOR4 BarycentricCoordinate(VECTOR4 a, VECTOR4 b, VECTOR4 c, VECTOR4 point);

float CalTriangleArea(VECTOR4 a, VECTOR4 b, VECTOR4 c);
VECTOR4 Vec3to4(VECTOR4* vec3);
VECTOR4 Color2Vec4(COLOR* color);