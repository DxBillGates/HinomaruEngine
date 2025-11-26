#include "../include/Quaternion.h"
#include "../include/Math.h"
#include <cmath>

using Matrix4x4 = GE::Math::Matrix4x4;
using Vector3 = GE::Math::Vector3;

GE::Math::Quaternion::Quaternion()
	:
	x(0), y(0), z(0), w(1)
{
}

GE::Math::Quaternion::Quaternion(float x, float y, float z, float w)
	:
	x(x), y(y), z(z), w(w)
{
}

GE::Math::Quaternion::Quaternion(const Vector3& v, float angle)
{
	float sin, cos;
	sin = std::sinf(angle / 2.0f);
	cos = std::cosf(angle / 2.0f);
	x = sin * v.x;
	y = sin * v.y;
	z = sin * v.z;
	w = cos;
}

GE::Math::Quaternion::Quaternion(const Matrix4x4& m)
{
	Quaternion result;
	float tr = m.m[0][0] + m.m[1][1] + m.m[2][2] + m.m[3][3];
	if (tr >= 1.0f)
	{
		float fourD = 2.0f * std::sqrtf(tr);
		result.x = (m.m[1][2] - m.m[2][1]) / fourD;
		result.y = (m.m[2][0] - m.m[0][2]) / fourD;
		result.z = (m.m[0][1] - m.m[1][0]) / fourD;
		result.w = fourD / 4.0f;
	}
	int i = 0, j = 0, k = 0;
	if (m.m[0][0] <= m.m[1][1])i = 1;
	if (m.m[2][2] > m.m[i][i])i = 2;
	j = (i + 1) % 3;
	k = (j + 1) % 3;
	tr = m.m[i][i] - m.m[j][j] - m.m[k][k] + 1.0f;
	float fourD = 2.0f * std::sqrtf(tr);
	float qa[4];
	qa[i] = fourD / 4.0f;
	qa[j] = (m.m[j][i] + m.m[i][j]) / fourD;
	qa[k] = (m.m[k][i] + m.m[i][k]) / fourD;
	qa[3] = (m.m[j][k] - m.m[k][j]) / fourD;

	x = qa[0];
	y = qa[1];
	z = qa[2];
	w = qa[3];
}

GE::Math::Quaternion GE::Math::Quaternion::Normalize(const Quaternion& q)
{
	Quaternion result = q;
	float length = Length(result);
	if (length != 0)
	{
		result /= length;
		return result;
	}
	return Quaternion();
}

GE::Math::Quaternion GE::Math::Quaternion::Conjugate(const Quaternion& q)
{
	Quaternion result = q;
	result = -result;
	result.w *= -1;
	return result;
}

float GE::Math::Quaternion::Dot(const Quaternion& q1, const Quaternion& q2)
{
	float result = 0;
	result = q1.x * q2.x + q1.y * q2.y + q1.z * q2.z + q1.w * q2.w;
	return result;
}

float GE::Math::Quaternion::Length(const Quaternion& q)
{
	float result = 0;
	result = std::sqrtf(q.x * q.x + q.y * q.y + q.z * q.z + q.w * q.w);
	return result;
}

GE::Math::Quaternion GE::Math::Quaternion::Slerp(const Quaternion& q1, const Quaternion& q2, float t)
{
	float cos = Dot(q1, q2);
	Quaternion t2 = q2;
	if (cos < 0.0f)
	{
		cos = -cos;
		Quaternion two = q2;
		t2 = -two;
	}
	float k0 = 1.0f - t;
	float k1 = t;
	if ((1.0f - cos) > 0.001f)
	{
		float theta = std::acosf(cos);
		k0 = std::sinf(theta * k0) / sin(theta);
		k1 = std::sinf(theta * k1) / sin(theta);
	}
	return q1 * k0 + t2 * k1;
}

GE::Math::Quaternion GE::Math::Quaternion::Lerp(const Quaternion& q1, const Quaternion& q2, float t)
{
	float cos = Dot(q1, q2);
	Quaternion t2 = q2;
	if (cos < 0.0f)
	{
		cos = -cos;
		Quaternion two = q2;
		t2 = -two;
	}
	float k0 = 1.0f - t;
	float k1 = t;
	return q1 * k0 + t2 * k1;
}

Matrix4x4 GE::Math::Quaternion::Rotation(const Quaternion& q)
{
	float xx = q.x * q.x * 2.0f;
	float yy = q.y * q.y * 2.0f;
	float zz = q.z * q.z * 2.0f;
	float xy = q.x * q.y * 2.0f;
	float xz = q.x * q.z * 2.0f;
	float yz = q.y * q.z * 2.0f;
	float wx = q.w * q.x * 2.0f;
	float wy = q.w * q.y * 2.0f;
	float wz = q.w * q.z * 2.0f;
	Matrix4x4 result =
	{
			1.0f - yy - zz,xy + wz,xz - wy,0.0f,
			xy - wz,1.0f - xx - zz,yz + wx,0.0f,
			xz + wy,yz - wx,1.0f - xx - yy,0.0f,
			0.0f,0.0f,0.0f,1.0f
	};
	return result;
}

Matrix4x4 GE::Math::Quaternion::Rotation()
{
	return Quaternion::Rotation(*this);
}

Vector3 GE::Math::Quaternion::GetAxis(const Quaternion& q)
{
	Vector3 result;
	float x, y, z, length;
	x = q.x;
	y = q.y;
	z = q.z;
	length = Length(q);
	result =
	{
		x / length,y / length,z / length
	};

	return result;
}

GE::Math::Quaternion GE::Math::Quaternion::Euler(const Math::Vector3& angle)
{
	float xRadian = Math::ConvertToRadian(angle.x);
	float yRadian = Math::ConvertToRadian(angle.y);
	float zRadian = Math::ConvertToRadian(angle.z);

	float xCos = std::cosf(xRadian / 2.0f);
	float xSin = std::sinf(xRadian / 2.0f);
	float yCos = std::cosf(yRadian / 2.0f);
	float ySin = std::sinf(yRadian / 2.0f);
	float zCos = std::cosf(zRadian / 2.0f);
	float zSin = std::sinf(zRadian / 2.0f);

	Quaternion result =
	{
		xSin * yCos * zCos - xCos * ySin * zSin,
		xCos * ySin * zCos + xSin * yCos * zSin,
		xCos * yCos * zSin - xSin * ySin * zCos,
		xCos * yCos * zCos + xSin * ySin * zSin,
	};
	return result;
}

GE::Math::Quaternion GE::Math::Quaternion::operator+()
{
	return *this;
}

GE::Math::Quaternion GE::Math::Quaternion::operator-()
{
	this->x = -this->x;
	this->y = -this->y;
	this->z = -this->z;
	this->w = -this->w;
	return *this;
}

GE::Math::Quaternion& GE::Math::Quaternion::operator+=(const Quaternion& q)
{
	this->x += q.x;
	this->y += q.y;
	this->z += q.z;
	this->w += q.w;
	return *this;
}

GE::Math::Quaternion& GE::Math::Quaternion::operator-=(const Quaternion& q)
{
	this->x -= q.x;
	this->y -= q.y;
	this->z -= q.z;
	this->w -= q.w;
	return *this;
}

GE::Math::Quaternion& GE::Math::Quaternion::operator*=(float s)
{
	this->x *= s;
	this->y *= s;
	this->z *= s;
	this->w *= s;
	return *this;
}

GE::Math::Quaternion& GE::Math::Quaternion::operator/=(float s)
{
	return *this *= 1.0f / s;
}

GE::Math::Quaternion& GE::Math::Quaternion::operator*=(const Quaternion& q)
{
	Quaternion result =
	{
		 this->x * q.w + this->y * q.z - this->z * q.y + this->w * q.x,
		-this->x * q.z + this->y * q.w + this->z * q.x + this->w * q.y,
		 this->x * q.y - this->y * q.x + this->z * q.w + this->w * q.z,
		-this->x * q.x - this->y * q.y - this->z * q.z + this->w * q.w,
	};
	*this = result;
	return *this;
}

GE::Math::Quaternion GE::Math::operator+(const Quaternion& q1, const Quaternion& q2)
{
	Quaternion result = q1;
	return result += q2;
}

GE::Math::Quaternion GE::Math::operator-(const Quaternion& q1, const Quaternion& q2)
{
	Quaternion result = q1;
	return result -= q2;
}

GE::Math::Quaternion GE::Math::operator*(const Quaternion& q1, const Quaternion& q2)
{
	Quaternion result = q1;
	return result *= q2;
}

GE::Math::Quaternion GE::Math::operator*(const Quaternion& q, float s)
{
	Quaternion result = q;
	return result *= s;
}

GE::Math::Quaternion GE::Math::operator*(float s, const Quaternion& q)
{
	Quaternion result = q;
	return result *= s;
}

GE::Math::Quaternion GE::Math::operator/(const Quaternion& q, float s)
{
	Quaternion result = q;
	return result /= s;
}
