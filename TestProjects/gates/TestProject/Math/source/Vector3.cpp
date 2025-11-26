#include "../include/Vector3.h"
#include <cmath>

GE::Math::Vector3::Vector3() :x(0), y(0), z(0)
{
}

GE::Math::Vector3::Vector3(float xyz) : x(xyz), y(xyz), z(xyz)
{
}

GE::Math::Vector3::Vector3(float x, float y, float z) : x(x), y(y), z(z)
{
}

float GE::Math::Vector3::Length() const
{
	return std::sqrtf(x * x + y * y + z * z);
}

GE::Math::Vector3 GE::Math::Vector3::Normalize()
{
	float length = Length();
	if (length == 0)
	{
		return Vector3();
	}
	return *this / length;
}

float GE::Math::Vector3::Distance(const Vector3 & v1, const Vector3 & v2)
{
	Vector3 v = v1 - v2;
	return v.Length();;
}

GE::Math::Vector3 GE::Math::Vector3::Normalize(const Vector3 & v)
{
	Vector3 v1 = v;
	float length = v1.Length();
	if (length == 0)
	{
		return Vector3();
	}
	return v1 / length;
}

GE::Math::Vector3 GE::Math::Vector3::Cross(const Vector3 & v1, const Vector3 & v2)
{
	Vector3 v = Vector3(v1.y*v2.z - v1.z*v2.y, v1.z*v2.x - v1.x*v2.z, v1.x*v2.y - v1.y*v2.x);
	return v;
}

float GE::Math::Vector3::Dot(const Vector3 & v1, const Vector3 & v2)
{
	float value = v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
	return value;
}

GE::Math::Vector3 GE::Math::Vector3::Lerp(const Vector3 & s, const Vector3 & e, float t)
{
	return s * (1.0f - t) + e * t;
}

GE::Math::Vector3 GE::Math::Vector3::Max(const Vector3& v1, const Vector3& v2)
{
	Vector3 result;
	result.x = (v1.x >= v2.x) ? v2.x : v1.x;
	result.y = (v1.y >= v2.y) ? v2.y : v1.y;
	result.z = (v1.z >= v2.z) ? v2.z : v1.z;
	return result;
}

GE::Math::Vector3 GE::Math::Vector3::Min(const Vector3& v1, const Vector3& v2)
{
	Vector3 result;
	result.x = (v1.x <= v2.x) ? v2.x : v1.x;
	result.y = (v1.y <= v2.y) ? v2.y : v1.y;
	result.z = (v1.z <= v2.z) ? v2.z : v1.z;
	return result;
}

GE::Math::Vector3 GE::Math::Vector3::operator+()
{
	return *this;
}

GE::Math::Vector3 GE::Math::Vector3::operator-()
{
	return Vector3(-x, -y, -z);
}

GE::Math::Vector3 & GE::Math::Vector3::operator+=(const Vector3 & v)
{
	x += v.x;
	y += v.y;
	z += v.z;
	return *this;
}

GE::Math::Vector3 & GE::Math::Vector3::operator-=(const Vector3 & v)
{
	x -= v.x;
	y -= v.y;
	z -= v.z;
	return *this;
}

GE::Math::Vector3 & GE::Math::Vector3::operator*=(float s)
{
	x *= s;
	y *= s;
	z *= s;
	return *this;
}

GE::Math::Vector3& GE::Math::Vector3::operator*=(const Vector3& v)
{
	x *= v.x;
	y *= v.y;
	z *= v.z;
	return *this;
}

GE::Math::Vector3 & GE::Math::Vector3::operator/=(float s)
{
	x /= s;
	y /= s;
	z /= s;
	return *this;
}

const GE::Math::Vector3 GE::Math::operator+(const Vector3 & v1, const Vector3 & v2)
{
	Vector3 a(v1);
	return a += v2;
}

const GE::Math::Vector3 GE::Math::operator-(const Vector3 & v1, const Vector3 & v2)
{
	Vector3 a(v1);
	return a -= v2;
}

const GE::Math::Vector3 GE::Math::operator*(const Vector3& v, float s)
{
	Vector3 a(v);
	return a *= s;
}

const GE::Math::Vector3 GE::Math::operator*(float s, const Vector3& v)
{
	return v * s;
}

const GE::Math::Vector3 GE::Math::operator*(const Vector3& v1, const Vector3& v2)
{
	Vector3 a(v1);
	return a *= v2;
}

const GE::Math::Vector3 GE::Math::operator/(const Vector3& v, float s)
{
	Vector3 a(v);
	return a /= s;
}

const GE::Math::Vector3 GE::Math::operator/(float s, const Vector3& v)
{

	return v / s;
}