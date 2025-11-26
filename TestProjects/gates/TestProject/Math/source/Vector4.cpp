#include "../include/Vector4.h"
#include <math.h>

GE::Math::Vector4::Vector4()
	:x(0), y(0), z(0), w(0)
{
}

GE::Math::Vector4::Vector4(float xyzw): x(xyzw), y(xyzw), z(xyzw), w(xyzw)
{
}

GE::Math::Vector4::Vector4(float x, float y, float z, float w)
	: x(x), y(y), z(z), w(w)
{
}

GE::Math::Vector4::Vector4(const Vector3& v)
	: x(v.x), y(v.y), z(v.z), w(0)
{
}

float GE::Math::Vector4::Length()
{
	return sqrtf(x * x + y * y + z * z);
}

GE::Math::Vector4 GE::Math::Vector4::Normalize()
{
	return *this / Length();
}

GE::Math::Vector4 GE::Math::Vector4::Normalize(const Vector4& v)
{
	Vector4 v1 = v;
	return v1 / v1.Length();
}

GE::Math::Vector4 GE::Math::Vector4::operator+()
{
	return *this;
}

GE::Math::Vector4 GE::Math::Vector4::operator-()
{
	return Vector4(-x, -y, -z, -w);
}

GE::Math::Vector4 & GE::Math::Vector4::operator+=(const Vector4 & v)
{
	x += v.x;
	y += v.y;
	z += v.z;
	w += v.w;
	return *this;
}

GE::Math::Vector4 & GE::Math::Vector4::operator-=(const Vector4 & v)
{
	x -= v.x;
	y -= v.y;
	z -= v.z;
	w -= v.w;
	return *this;
}

GE::Math::Vector4 & GE::Math::Vector4::operator*=(float s)
{
	x *= s;
	y *= s;
	z *= s;
	w *= s;
	return *this;
}

GE::Math::Vector4 & GE::Math::Vector4::operator/=(float s)
{
	x /= s;
	y /= s;
	z /= s;
	w /= s;
	return *this;
}

const GE::Math::Vector4 GE::Math::operator+(const Vector4 & v1, const Vector4 & v2)
{
	Vector4 a(v1);
	return a += v2;
}

const GE::Math::Vector4 GE::Math::operator-(const Vector4 & v1, const Vector4 & v2)
{
	Vector4 a(v1);
	return a -= v2;
}

const GE::Math::Vector4 GE::Math::operator*(const Vector4& v, float s)
{
	Vector4 a(v);
	return a *= s;
}

const GE::Math::Vector4 GE::Math::operator*(float s, const Vector4& v)
{
	return v * s;
}

const GE::Math::Vector4 GE::Math::operator/(const Vector4& v, float s)
{
	Vector4 a(v);
	return a /= s;
}

const GE::Math::Vector4 GE::Math::operator/(float s, const Vector4& v)
{
	return v / s;
}
