#include"Math.h"
VECTOR3::VECTOR3(float x, float y, float z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}
//ÖØÔØÔËËã·û
VECTOR3 VECTOR3::operator + (VECTOR3 vec)
{
	VECTOR3 res;
	res.x = this->x + vec.x;
	res.y = this->y + vec.y;
	res.z = this->z + vec.z;
	return res;
}
VECTOR3 VECTOR3::operator - (VECTOR3 vec)
{
	VECTOR3 res;
	res.x = this->x - vec.x;
	res.y = this->y - vec.y;
	res.z = this->z - vec.z;
	return res;
}
VECTOR3 VECTOR3::operator * (float f)
{
	VECTOR3 res;
	res.x = this->x * f;
	res.y = this->y * f;
	res.z = this->z * f;
	return res;
}
VECTOR3 VECTOR3::operator / (float f)
{
	VECTOR3 res;
	res.x = this->x / f;
	res.y = this->y / f;
	res.z = this->z / f;
	return res;
}
void    VECTOR3::operator = (const VECTOR3 vec)
{
	this->x = vec.x;
	this->y = vec.y;
	this->z = vec.z;
}

VECTOR4::VECTOR4(float x, float y, float z, float w)
{
	this->x = x;
	this->y = y;
	this->z = z;
	this->w = w;
}
VECTOR4 VECTOR4::operator + (VECTOR4 vec)
{
	VECTOR4 res;
	res.x = this->x + vec.x;
	res.y = this->y + vec.y;
	res.z = this->z + vec.z;
	res.w = this->w + vec.w;
	return res;
}
VECTOR4 VECTOR4::operator - (VECTOR4 vec)
{
	VECTOR4 res;
	res.x = this->x - vec.x;
	res.y = this->y - vec.y;
	res.z = this->z - vec.z;
	res.w = this->w - vec.w;
	return res;
}
VECTOR4 VECTOR4::operator * (float f)
{
	VECTOR4 res;
	res.x = this->x * f;
	res.y = this->y * f;
	res.z = this->z * f;
	res.w = this->w * f;
	return res;
}
VECTOR4 VECTOR4::operator / (float f)
{
	VECTOR4 res;
	res.x = this->x / f;
	res.y = this->y / f;
	res.z = this->z / f;
	res.w = this->w / f;
	return res;
}
void    VECTOR4::operator = (const VECTOR4 vec)
{
	this->x = vec.x;
	this->y = vec.y;
	this->z = vec.z;
	this->w = vec.w;
}


MATRIX::MATRIX(float _11, float _12, float _13, float _14,
	float _21, float _22, float _23, float _24,
	float _31, float _32, float _33, float _34,
	float _41, float _42, float _43, float _44)
{
	this->_11 = _11; this->_12 = _12; this->_13 = _13; this->_14 = _14;
	this->_21 = _21; this->_22 = _22; this->_23 = _23; this->_24 = _24;
	this->_31 = _31; this->_32 = _32; this->_33 = _33; this->_34 = _34;
	this->_41 = _41; this->_42 = _42; this->_43 = _43; this->_44 = _44;
}
MATRIX::MATRIX(VECTOR4 v1, VECTOR4 v2, VECTOR4 v3)
{
	this->_11 = v1.x; this->_12 = v1.y; this->_13 = v1.z; this->_14 = 0;
	this->_21 = v2.x; this->_22 = v2.y; this->_23 = v2.z; this->_24 = 0;
	this->_31 = v3.x; this->_32 = v3.y; this->_33 = v3.z; this->_34 = 0;
	this->_41 = 0;	  this->_42 = 0;    this->_43 = 0;    this->_44 = 0;
}

MATRIX MATRIX::operator * (MATRIX matrix)
{
	MATRIX res;
	res._11 = this->_11 * matrix._11 + this->_12 * matrix._21 + this->_13 * matrix._31 + this->_14 * matrix._41;
	res._12 = this->_11 * matrix._12 + this->_12 * matrix._22 + this->_13 * matrix._32 + this->_14 * matrix._42;
	res._13 = this->_11 * matrix._13 + this->_12 * matrix._23 + this->_13 * matrix._33 + this->_14 * matrix._43;
	res._14 = this->_11 * matrix._14 + this->_12 * matrix._24 + this->_13 * matrix._34 + this->_14 * matrix._44;
	res._21 = this->_21 * matrix._11 + this->_22 * matrix._21 + this->_23 * matrix._31 + this->_24 * matrix._41;
	res._22 = this->_21 * matrix._12 + this->_22 * matrix._22 + this->_23 * matrix._32 + this->_24 * matrix._42;
	res._23 = this->_21 * matrix._13 + this->_22 * matrix._23 + this->_23 * matrix._33 + this->_24 * matrix._43;
	res._24 = this->_21 * matrix._14 + this->_22 * matrix._24 + this->_23 * matrix._34 + this->_24 * matrix._44;
	res._31 = this->_31 * matrix._11 + this->_32 * matrix._21 + this->_33 * matrix._31 + this->_34 * matrix._41;
	res._32 = this->_31 * matrix._12 + this->_32 * matrix._22 + this->_33 * matrix._32 + this->_34 * matrix._42;
	res._33 = this->_31 * matrix._13 + this->_32 * matrix._23 + this->_33 * matrix._33 + this->_34 * matrix._43;
	res._34 = this->_31 * matrix._14 + this->_32 * matrix._24 + this->_33 * matrix._34 + this->_34 * matrix._44;
	res._41 = this->_41 * matrix._11 + this->_42 * matrix._21 + this->_43 * matrix._31 + this->_44 * matrix._41;
	res._42 = this->_41 * matrix._12 + this->_42 * matrix._22 + this->_43 * matrix._32 + this->_44 * matrix._42;
	res._43 = this->_41 * matrix._13 + this->_42 * matrix._23 + this->_43 * matrix._33 + this->_44 * matrix._43;
	res._44 = this->_41 * matrix._14 + this->_42 * matrix._24 + this->_43 * matrix._34 + this->_44 * matrix._44;
	return res;
}
MATRIX MATRIX::operator + (MATRIX matrix)
{
	MATRIX res;
	res._11 = this->_11 + matrix._11;
	res._12 = this->_12 + matrix._12;
	res._13 = this->_13 + matrix._13;
	res._14 = this->_14 + matrix._14;
	res._21 = this->_21 + matrix._21;
	res._22 = this->_22 + matrix._22;
	res._23 = this->_23 + matrix._23;
	res._24 = this->_24 + matrix._24;
	res._31 = this->_31 + matrix._31;
	res._32 = this->_32 + matrix._32;
	res._33 = this->_33 + matrix._33;
	res._34 = this->_34 + matrix._34;
	res._41 = this->_41 + matrix._41;
	res._42 = this->_42 + matrix._42;
	res._43 = this->_43 + matrix._43;
	res._44 = this->_44 + matrix._44;
	return res;
}
MATRIX MATRIX::operator - (MATRIX matrix)
{
	MATRIX res;
	res._11 = this->_11 - matrix._11;
	res._12 = this->_12 - matrix._12;
	res._13 = this->_13 - matrix._13;
	res._14 = this->_14 - matrix._14;
	res._21 = this->_21 - matrix._21;
	res._22 = this->_22 - matrix._22;
	res._23 = this->_23 - matrix._23;
	res._24 = this->_24 - matrix._24;
	res._31 = this->_31 - matrix._31;
	res._32 = this->_32 - matrix._32;
	res._33 = this->_33 - matrix._33;
	res._34 = this->_34 - matrix._34;
	res._41 = this->_41 - matrix._41;
	res._42 = this->_42 - matrix._42;
	res._43 = this->_43 - matrix._43;
	res._44 = this->_44 - matrix._44;
	return res;
}
MATRIX MATRIX::operator * (float f)
{
	MATRIX res;
	res._11 = this->_11 * f;
	res._12 = this->_12 * f;
	res._13 = this->_13 * f;
	res._14 = this->_14 * f;
	res._21 = this->_21 * f;
	res._22 = this->_22 * f;
	res._23 = this->_23 * f;
	res._24 = this->_24 * f;
	res._31 = this->_31 * f;
	res._32 = this->_32 * f;
	res._33 = this->_33 * f;
	res._34 = this->_34 * f;
	res._41 = this->_41 * f;
	res._42 = this->_42 * f;
	res._43 = this->_43 * f;
	res._44 = this->_44 * f;
	return res;
}
MATRIX MATRIX::operator / (float f)
{
	MATRIX res;
	res._11 = this->_11 / f;
	res._12 = this->_12 / f;
	res._13 = this->_13 / f;
	res._14 = this->_14 / f;
	res._21 = this->_21 / f;
	res._22 = this->_22 / f;
	res._23 = this->_23 / f;
	res._24 = this->_24 / f;
	res._31 = this->_31 / f;
	res._32 = this->_32 / f;
	res._33 = this->_33 / f;
	res._34 = this->_34 / f;
	res._41 = this->_41 / f;
	res._42 = this->_42 / f;
	res._43 = this->_43 / f;
	res._44 = this->_44 / f;
	return res;
}
void   MATRIX::operator = (const MATRIX matrix)
{
	this->_11 = matrix._11;
	this->_12 = matrix._12;
	this->_13 = matrix._13;
	this->_14 = matrix._14;
	this->_21 = matrix._21;
	this->_22 = matrix._22;
	this->_23 = matrix._23;
	this->_24 = matrix._24;
	this->_31 = matrix._31;
	this->_32 = matrix._32;
	this->_33 = matrix._33;
	this->_34 = matrix._34;
	this->_41 = matrix._41;
	this->_42 = matrix._42;
	this->_43 = matrix._43;
	this->_44 = matrix._44;
}

COLOR::COLOR(int r, int g, int b, int w)
{
	this->r = r;
	this->g = g;
	this->b = b;
	this->w = w;
}
COLOR COLOR::operator + (COLOR vec)
{
	COLOR res;
	res.r = this->r + vec.r;
	res.g = this->g + vec.g;
	res.b = this->b + vec.b;
	res.w = this->w + vec.w;
	return res;
}
COLOR COLOR::operator - (COLOR vec)
{
	COLOR res;
	res.r = this->r - vec.r;
	res.g = this->g - vec.g;
	res.b = this->b - vec.b;
	res.w = this->w - vec.w;
	return res;
}
COLOR COLOR::operator * (float f)
{
	COLOR res;
	res.r = this->r * f;
	res.g = this->g * f;
	res.b = this->b * f;
	res.w = this->w * f;
	return res;
}
COLOR COLOR::operator / (float f)
{
	COLOR res;
	res.r = this->r / f;
	res.g = this->g / f;
	res.b = this->b / f;
	res.w = this->w / f;
	return res;
}
void  COLOR::operator = (const COLOR v)
{
	this->r = v.r;
	this->g = v.g;
	this->b = v.b;
	this->w = v.w;
}
bool  COLOR::operator == (const COLOR v)
{
	if (this->r == v.r && this->g == v.g && this->b == v.b && this->w == v.w) {
		return true;
	}
	else
	{
		return false;
	}
}

VECTOR3 Vec3Cross(VECTOR3* pv1, VECTOR3* pv2)
{
	VECTOR3 res;
	res.x = pv1->y * pv2->z - pv2->y * pv1->z;
	res.y = pv1->x * pv2->z - pv2->x * pv1->z;
	res.z = pv1->x * pv2->y - pv2->x * pv1->y;
	return res;
}
float Vec3Dot(VECTOR3* pv1, VECTOR3* pv2)
{
	float res;
	res = pv1->x * pv2->x + pv1->y * pv2->y + pv1->z * pv2->z;
	return res;
}
float Vec3Length(VECTOR3* pv)
{
	float res;
	res = sqrt(pv->x * pv->x + pv->y * pv->y + pv->z * pv->z);
	return res;
}
VECTOR3 Vec3Normalize(VECTOR3* pv)
{
	VECTOR3 res;
	res.x = pv->x / Vec3Length(pv);
	res.y = pv->y / Vec3Length(pv);
	res.z = pv->z / Vec3Length(pv);
	return res;
}

float Vec4Dot(VECTOR4* pv1, VECTOR4* pv2)
{
	float res;
	res = pv1->x * pv2->x + pv1->y * pv2->y + pv1->z * pv2->z + pv1->w * pv2->w;
	return res;
}
float Vec4Length(VECTOR4* pv)
{
	float res;
	res = sqrt(pv->x * pv->x + pv->y * pv->y + pv->z * pv->z + pv->w * pv->w);
	return res;
}
VECTOR4 Vec4Transform(VECTOR4* pv, MATRIX* pm)
{
	VECTOR4 res;
	res.x = pv->x * pm->_11 + pv->y * pm->_21 + pv->z * pm->_31 + pv->w * pm->_41;
	res.y = pv->x * pm->_12 + pv->y * pm->_22 + pv->z * pm->_32 + pv->w * pm->_42;
	res.z = pv->x * pm->_13 + pv->y * pm->_23 + pv->z * pm->_33 + pv->w * pm->_43;
	res.w = pv->x * pm->_14 + pv->y * pm->_24 + pv->z * pm->_34 + pv->w * pm->_44;
	return res;
}
VECTOR4 Vec4Normalize(VECTOR4* pv)
{
	VECTOR4 res;
	res.x = pv->x / Vec4Length(pv);
	res.y = pv->y / Vec4Length(pv);
	res.z = pv->z / Vec4Length(pv);
	res.w = pv->w / Vec4Length(pv);
	return res;
}
VECTOR4 Vec4Cross(VECTOR4* pv1, VECTOR4* pv2)
{
	VECTOR4 res;
	res.x = pv1->y * pv2->z - pv2->y * pv1->z;
	res.y = pv1->x * pv2->z - pv2->x * pv1->z;
	res.z = pv1->x * pv2->y - pv2->x * pv1->y;
	res.w = 0;
	return res;
}

MATRIX MatrixIdentity()
{
	MATRIX res = MATRIX(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
	return res;
}
MATRIX MatrixRotationX(float Angle)
{
	MATRIX res;
	res._11 = 1;
	res._22 = cos(Angle);
	res._23 = -sin(Angle);
	res._32 = sin(Angle);
	res._33 = cos(Angle);
	res._44 = 1;
	return res;
}
MATRIX MatrixRotationY(float Angle)
{
	MATRIX res;
	res._11 = cos(Angle);
	res._13 = sin(Angle);
	res._22 = 1;
	res._31 = -sin(Angle);
	res._33 = cos(Angle);
	res._44 = 1;
	return res;
}
MATRIX MatrixRotationZ(float Angle)
{
	MATRIX res;
	res._11 = cos(Angle);
	res._12 = -sin(Angle);
	res._21 = sin(Angle);
	res._22 = cos(Angle);
	res._33 = 1;
	res._44 = 1;
	return res;
}
MATRIX MatrixScalling(float sx, float sy, float sz)
{
	MATRIX res;
	res._11 = sx;
	res._22 = sy;
	res._33 = sz;
	res._44 = 1;
	return res;
}
MATRIX MatrixLookAtLH(VECTOR4* pEye, VECTOR4* pAt, VECTOR4* pUp)
{
	MATRIX res;
	//先计算出视图片空间各坐标轴的向量，单位向量
	VECTOR4 temp = *pAt - *pEye;
	VECTOR4 cameraZaxis = Vec4Normalize(&temp);
	temp = Vec4Cross(pUp, &cameraZaxis);
	VECTOR4 cameraXaxis = Vec4Normalize(&temp);
	VECTOR4 cameraYaxis = Vec4Cross(&cameraZaxis, &cameraXaxis);
	float DXP = -Vec4Dot(&cameraXaxis, pEye);
	float DYP = -Vec4Dot(&cameraYaxis, pEye);
	float DZP = -Vec4Dot(&cameraZaxis, pEye);
	res._11 = cameraXaxis.x;
	res._12 = cameraYaxis.x;
	res._13 = cameraZaxis.x;
	res._14 = 0;
	res._21 = cameraXaxis.y;
	res._22 = cameraYaxis.y;
	res._23 = cameraZaxis.y;
	res._24 = 0;
	res._31 = cameraXaxis.z;
	res._32 = cameraYaxis.z;
	res._33 = cameraZaxis.z;
	res._34 = 0;
	res._41 = DXP;
	res._42 = DYP;
	res._43 = DZP;
	res._44 = 1;
	return res;
}
MATRIX MatrixPerspectiveLH(float fovy, float Aspect, float zn, float zf)
{
	MATRIX res;
	res._11 = 1 / (Aspect * tan(fovy / 2));
	res._22 = 1 / tan(fovy / 2);
	res._33 = zf / (zf - zn);
	res._34 = 1;
	res._43 = -(zn * zf) / (zf - zn);
	return res;
}
VECTOR4 Wnormalization(VECTOR4* pv)
{
	VECTOR4 res;
	res.w = 1 / pv->z;//存顶点在透视空间的1/z，用于后续透视矫正
	res.x = pv->x / pv->w;
	res.y = pv->y / pv->w;
	res.z = pv->z / pv->w;
	return res;
}

VECTOR3 Vec4toVec2d(VECTOR4 vec4)
{
	VECTOR3 vec3;
	vec3.x = vec4.x;
	vec3.y = vec4.y;
	vec3.z = 1;
	return vec3;
}
float CalPlanePointZ(VECTOR4 v1, VECTOR4 v2, VECTOR4 v3, VECTOR4 vp)//已知三点坐标，求面上另一点已知x，y的坐标的z
{
	//求平面法向量
	float na = (v2.y - v1.y) * (v3.z - v1.z) - (v2.z - v1.z) * (v3.y - v1.y);
	float nb = (v2.z - v1.z) * (v3.x - v1.x) - (v2.x - v1.x) * (v3.z - v1.z);
	float nc = (v2.x - v1.x) * (v3.y - v1.y) - (v2.y - v1.y) * (v3.x - v1.x);
	VECTOR4 vn = VECTOR4(na, nb, nc, 0);
	if (vn.z != 0)
	{
		vp.z = v1.z - (vn.x * (vp.x - v1.x) + vn.y * (vp.y - v1.y)) / vn.z;
	}
	else
	{
		vp.z = 1;//平面平行于z轴，指定x、y后，z有无数个值
	}
	return vp.z;
}
VECTOR4 BarycentricCoordinate(VECTOR4 A, VECTOR4 B, VECTOR4 C, VECTOR4 point)//计算三角形某一点的重心坐标
{
	float a = (-(point.x - B.x) * (C.y - B.y) + (point.y - B.y) * (C.x - B.x)) / (-(A.x - B.x) * (C.y - B.y) + (A.y - B.y) * (C.x - B.x));
	float b = (-(point.x - C.x) * (A.y - C.y) + (point.y - C.y) * (A.x - C.x)) / (-(B.x - C.x) * (A.y - C.y) + (B.y - C.y) * (A.x - C.x));
	float y = 1 - a - b;
	return VECTOR4(a, b, y, 1);
}

float CalTriangleArea(VECTOR4 a, VECTOR4 b, VECTOR4 c)//计算三角形的面积
{
	float side[3];
	side[0] = sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2) + pow(a.z - b.z, 2));
	side[1] = sqrt(pow(a.x - c.x, 2) + pow(a.y - c.y, 2) + pow(a.z - c.z, 2));
	side[2] = sqrt(pow(c.x - b.x, 2) + pow(c.y - b.y, 2) + pow(c.z - b.z, 2));
	float p = (side[0] + side[1] + side[2]) / 2;
	float area = sqrt(p * (p - side[0]) * (p - side[1]) * (p - side[2]));
	return area;
}
VECTOR4 Vec3to4(VECTOR4* vec3)
{
	VECTOR4 vec4;
	vec4.x = vec3->x;
	vec4.y = vec3->y;
	vec4.z = vec3->z;
	vec4.w = 0;
	return vec4;
}
VECTOR4 Color2Vec4(COLOR* color)
{
	VECTOR4 vec4;
	vec4.x = color->r;
	vec4.y = color->g;
	vec4.z = color->b;
	vec4.w = color->w;
	return vec4;
}