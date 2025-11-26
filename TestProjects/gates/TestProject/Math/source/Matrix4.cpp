#include "../include/Matrix4.h"
#include "../include/Math.h"
#include <cmath>

using Vector3 = GE::Math::Vector3;

GE::Math::Matrix4x4::Matrix4x4()
{
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			m[i][j] = 0;
		}
	}
}

GE::Math::Matrix4x4::Matrix4x4(
	float m00, float m01, float m02, float m03,
	float m10, float m11, float m12, float m13,
	float m20, float m21, float m22, float m23,
	float m30, float m31, float m32, float m33)
	: _11(m00), _12(m01), _13(m02), _14(m03),
	_21(m10), _22(m11), _23(m12), _24(m13),
	_31(m20), _32(m21), _33(m22), _34(m23),
	_41(m30), _42(m31), _43(m32), _44(m33)
{
}

GE::Math::Matrix4x4 GE::Math::Matrix4x4::Identity()
{
	Matrix4x4 result
	{
		1.0f,0.0f,0.0f,0.0f,
		0.0f,1.0f,0.0f,0.0f,
		0.0f,0.0f,1.0f,0.0f,
		0.0f,0.0f,0.0f,1.0f,
	};

	return result;
}

GE::Math::Axis GE::Math::Matrix4x4::GetAxis(const Matrix4x4& m)
{
	Axis result;
	result.x = Vector3(m._11, m._12, m._13).Normalize();
	result.y = Vector3(m._21, m._22, m._23).Normalize();
	result.z = Vector3(m._31, m._32, m._33).Normalize();
	return result;
}

GE::Math::Matrix4x4 GE::Math::Matrix4x4::Scale(const Vector3 & v)
{
	Matrix4x4 result
	{
		v.x,0.0f,0.0f,0.0f,
		0.0f,v.y,0.0f,0.0f,
		0.0f,0.0f,v.z,0.0f,
		0.0f,0.0f,0.0f,1.0f,
	};

	return result;
}

GE::Math::Matrix4x4 GE::Math::Matrix4x4::RotationX(float angle)
{
	float sin = std::sinf(angle);
	float cos = std::cosf(angle);

	Matrix4x4 result
	{
		1.0f,0.0f,0.0f,0.0f,
		0.0f,cos,sin,0.0f,
		0.0f,-sin,cos,0.0f,
		0.0f,0.0f,0.0f,1.0f,
	};

	return result;
}

GE::Math::Matrix4x4 GE::Math::Matrix4x4::RotationY(float angle)
{
	float sin = std::sinf(angle);
	float cos = std::cosf(angle);

	Matrix4x4 result
	{
		cos,0.0f,-sin,0.0f,
		0.0f,1.0f,0.0f,0.0f,
		sin,0.0f,cos,0.0f,
		0.0f,0.0f,0.0f,1.0f,
	};

	return result;
}

GE::Math::Matrix4x4 GE::Math::Matrix4x4::RotationZ(float angle)
{
	float sin = std::sinf(angle);
	float cos = std::cosf(angle);

	Matrix4x4 result
	{
		cos,sin,0.0f,0.0f,
		-sin,cos,0.0f ,0.0f,
		0.0f,0.0f,1.0f,0.0f,
		0.0f,0.0f,0.0f,1.0f,
	};

	return result;
}

GE::Math::Matrix4x4 GE::Math::Matrix4x4::RotationZXY(const Vector3& angles)
{
	return RotationZ(Math::ConvertToRadian(angles.z)) * RotationX(Math::ConvertToRadian(angles.x)) * RotationY(Math::ConvertToRadian(angles.y));
}

GE::Math::Matrix4x4 GE::Math::Matrix4x4::Translate(const Vector3 & v)
{
	Matrix4x4 result
	{
		1.0f,0.0f,0.0f,0.0f,
		0.0f,1.0f,0.0f,0.0f,
		0.0f,0.0f,1.0f,0.0f,
		v.x,v.y,v.z,1.0f,
	};

	return result;
}

Vector3 GE::Math::Matrix4x4::Transform(const Vector3 & v, const Matrix4x4 & m)
{
	float w = v.x * m.m[0][3] + v.y * m.m[1][3] + v.z * m.m[2][3] + m.m[3][3];

	Vector3 result
	{
		(v.x * m.m[0][0] + v.y * m.m[1][0] + v.z * m.m[2][0] + m.m[3][0]) / w,
		(v.x * m.m[0][1] + v.y * m.m[1][1] + v.z * m.m[2][1] + m.m[3][1]) / w,
		(v.x * m.m[0][2] + v.y * m.m[1][2] + v.z * m.m[2][2] + m.m[3][2]) / w,
	};

	return result;
}

GE::Math::Matrix4x4 GE::Math::Matrix4x4::Transpose(const Matrix4x4 & m)
{
	Matrix4x4 result;
	result =
	{
		m.m[0][0],m.m[1][0],m.m[2][0],m.m[3][0],
		m.m[0][1],m.m[1][1],m.m[2][1],m.m[3][1],
		m.m[0][2],m.m[1][2],m.m[2][2],m.m[3][2],
		m.m[0][3],m.m[1][3],m.m[2][3],m.m[3][3],
	};
	return result;
}

GE::Math::Matrix4x4 GE::Math::Matrix4x4::Inverse(const Matrix4x4 & m)
{
	Matrix4x4 result;
	result.m[0][0] = m.m[1][1] * m.m[2][2] * m.m[3][3] + m.m[1][2] * m.m[2][3] * m.m[3][1] + m.m[1][3] * m.m[2][1] * m.m[3][2] - m.m[1][3] * m.m[2][2] * m.m[3][1] - m.m[1][2] * m.m[2][1] * m.m[3][3] - m.m[1][1] * m.m[2][3] * m.m[3][2];
	result.m[0][1] = -m.m[0][1] * m.m[2][2] * m.m[3][3] - m.m[0][2] * m.m[2][3] * m.m[3][1] - m.m[0][3] * m.m[2][1] * m.m[3][2] + m.m[0][3] * m.m[2][2] * m.m[3][1] + m.m[0][2] * m.m[2][1] * m.m[3][3] + m.m[0][1] * m.m[2][3] * m.m[3][2];
	result.m[0][2] = m.m[0][1] * m.m[1][2] * m.m[3][3] + m.m[0][2] * m.m[1][3] * m.m[3][1] + m.m[0][3] * m.m[1][1] * m.m[3][2] - m.m[0][3] * m.m[1][2] * m.m[3][1] - m.m[0][2] * m.m[1][1] * m.m[3][3] - m.m[0][1] * m.m[1][3] * m.m[3][2];
	result.m[0][3] = -m.m[0][1] * m.m[1][2] * m.m[2][3] - m.m[0][2] * m.m[1][3] * m.m[2][1] - m.m[0][3] * m.m[1][1] * m.m[2][2] + m.m[0][3] * m.m[1][2] * m.m[2][1] + m.m[0][2] * m.m[1][1] * m.m[2][3] + m.m[0][1] * m.m[1][3] * m.m[2][2];

	result.m[1][0] = -m.m[1][0] * m.m[2][2] * m.m[3][3] - m.m[1][2] * m.m[2][3] * m.m[3][0] - m.m[1][3] * m.m[2][0] * m.m[3][2] + m.m[1][3] * m.m[2][2] * m.m[3][0] + m.m[1][2] * m.m[2][0] * m.m[3][3] + m.m[1][0] * m.m[2][3] * m.m[3][2];
	result.m[1][1] = m.m[0][0] * m.m[2][2] * m.m[3][3] + m.m[0][2] * m.m[2][3] * m.m[3][0] + m.m[0][3] * m.m[2][0] * m.m[3][2] - m.m[0][3] * m.m[2][2] * m.m[3][0] - m.m[0][2] * m.m[2][0] * m.m[3][3] - m.m[0][0] * m.m[2][3] * m.m[3][2];
	result.m[1][2] = -m.m[0][0] * m.m[1][2] * m.m[3][3] - m.m[0][2] * m.m[1][3] * m.m[3][0] - m.m[0][3] * m.m[1][0] * m.m[3][2] + m.m[0][3] * m.m[1][2] * m.m[3][0] + m.m[0][2] * m.m[1][0] * m.m[3][3] + m.m[0][0] * m.m[1][3] * m.m[3][2];
	result.m[1][3] = m.m[0][0] * m.m[1][2] * m.m[2][3] + m.m[0][2] * m.m[1][3] * m.m[2][0] + m.m[0][3] * m.m[1][0] * m.m[2][2] - m.m[0][3] * m.m[1][2] * m.m[2][0] - m.m[0][2] * m.m[1][0] * m.m[2][3] - m.m[0][0] * m.m[1][3] * m.m[2][2];

	result.m[2][0] = m.m[1][0] * m.m[2][1] * m.m[3][3] + m.m[1][1] * m.m[2][3] * m.m[3][0] + m.m[1][3] * m.m[2][0] * m.m[3][1] - m.m[1][3] * m.m[2][1] * m.m[3][0] - m.m[1][1] * m.m[2][0] * m.m[3][3] - m.m[1][0] * m.m[2][3] * m.m[3][1];
	result.m[2][1] = -m.m[0][0] * m.m[2][1] * m.m[3][3] - m.m[0][1] * m.m[2][3] * m.m[3][0] - m.m[0][3] * m.m[2][0] * m.m[3][1] + m.m[0][3] * m.m[2][1] * m.m[3][0] + m.m[0][1] * m.m[2][0] * m.m[3][3] + m.m[0][0] * m.m[2][3] * m.m[3][1];
	result.m[2][2] = m.m[0][0] * m.m[1][1] * m.m[3][3] + m.m[0][1] * m.m[1][3] * m.m[3][0] + m.m[0][3] * m.m[1][0] * m.m[3][1] - m.m[0][3] * m.m[1][1] * m.m[3][0] - m.m[0][1] * m.m[1][0] * m.m[3][3] - m.m[0][0] * m.m[1][3] * m.m[3][1];
	result.m[2][3] = -m.m[0][0] * m.m[1][1] * m.m[2][3] - m.m[0][1] * m.m[1][3] * m.m[2][0] - m.m[0][3] * m.m[1][0] * m.m[2][1] + m.m[0][3] * m.m[1][1] * m.m[2][0] + m.m[0][1] * m.m[1][0] * m.m[2][3] + m.m[0][0] * m.m[1][3] * m.m[2][1];

	result.m[3][0] = -m.m[1][0] * m.m[2][1] * m.m[3][2] - m.m[1][1] * m.m[2][2] * m.m[3][0] - m.m[1][2] * m.m[2][0] * m.m[3][1] + m.m[1][2] * m.m[2][1] * m.m[3][0] + m.m[1][1] * m.m[2][0] * m.m[3][2] + m.m[1][0] * m.m[2][2] * m.m[3][1];
	result.m[3][1] = m.m[0][0] * m.m[2][1] * m.m[3][2] + m.m[0][1] * m.m[2][2] * m.m[3][0] + m.m[0][2] * m.m[2][0] * m.m[3][1] - m.m[0][2] * m.m[2][1] * m.m[3][0] - m.m[0][1] * m.m[2][0] * m.m[3][2] - m.m[0][0] * m.m[2][2] * m.m[3][1];
	result.m[3][2] = -m.m[0][0] * m.m[1][1] * m.m[3][2] - m.m[0][1] * m.m[1][2] * m.m[3][0] - m.m[0][2] * m.m[1][0] * m.m[3][1] + m.m[0][2] * m.m[1][1] * m.m[3][0] + m.m[0][1] * m.m[1][0] * m.m[3][2] + m.m[0][0] * m.m[1][2] * m.m[3][1];
	result.m[3][3] = m.m[0][0] * m.m[1][1] * m.m[2][2] + m.m[0][1] * m.m[1][2] * m.m[2][0] + m.m[0][2] * m.m[1][0] * m.m[2][1] - m.m[0][2] * m.m[1][1] * m.m[2][0] - m.m[0][1] * m.m[1][0] * m.m[2][2] - m.m[0][0] * m.m[1][2] * m.m[2][1];

	float f = Matrix4x4::Determinant(m);
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			result.m[i][j] *= 1.0f / f;
		}
	}
	return result;
}

float GE::Math::Matrix4x4::Determinant(const Matrix4x4 & m)
{
	float f0 = m.m[0][0] * ((m.m[1][1] * (m.m[2][2] * m.m[3][3] - m.m[2][3] * m.m[3][2]) - (m.m[1][2] * (m.m[2][1] * m.m[3][3] - m.m[2][3] * m.m[3][1])) + (m.m[1][3] * (m.m[2][1] * m.m[3][2] - m.m[2][2] * m.m[3][1]))));
	float f1 = m.m[1][0] * ((m.m[0][1] * (m.m[2][2] * m.m[3][3] - m.m[2][3] * m.m[3][2]) - (m.m[0][2] * (m.m[2][1] * m.m[3][3] - m.m[2][3] * m.m[3][1])) + (m.m[0][3] * (m.m[2][1] * m.m[3][2] - m.m[2][2] * m.m[3][1]))));
	float f2 = m.m[2][0] * ((m.m[0][1] * (m.m[1][2] * m.m[3][3] - m.m[1][3] * m.m[3][2]) - (m.m[0][2] * (m.m[1][1] * m.m[3][3] - m.m[1][3] * m.m[3][1])) + (m.m[0][3] * (m.m[1][1] * m.m[3][2] - m.m[1][2] * m.m[3][1]))));
	float f3 = m.m[3][0] * ((m.m[0][1] * (m.m[1][2] * m.m[2][3] - m.m[1][3] * m.m[2][2]) - (m.m[0][2] * (m.m[1][1] * m.m[2][3] - m.m[1][3] * m.m[2][1])) + (m.m[0][3] * (m.m[1][1] * m.m[2][2] - m.m[1][2] * m.m[2][1]))));
	float result = f0 - f1 + f2 - f3;
	return result;
}

GE::Math::Axis GE::Math::Matrix4x4::GetAxis() const
{
	return GetAxis(*this);
}

GE::Math::Vector3 GE::Math::Matrix4x4::GetPosition()
{
	return Math::Vector3(_41,_42,_43);
}

GE::Math::Matrix4x4 GE::Math::Matrix4x4::GetViewMatrixLookAt(const Vector3& pos, const Vector3& focasPos, const Vector3& upDir)
{
	Vector3 dir = (focasPos - pos);
	return GetViewMatrixLookTo(pos,dir.Normalize(),upDir);
}

GE::Math::Matrix4x4 GE::Math::Matrix4x4::GetViewMatrixLookTo(const Vector3& pos, const Vector3& direction, const Vector3& upDir)
{
	Matrix4x4 result;
	Vector3 xAxis, yAxis, zAxis;
	zAxis = direction;
	xAxis = Vector3::Normalize(Vector3::Cross(upDir, zAxis));
	yAxis = Vector3::Cross(zAxis, xAxis);
	Vector3 negpos = Vector3(-pos.x, -pos.y, -pos.z);
	result =
	{
		xAxis.x,xAxis.y,xAxis.z,0,
		yAxis.x,yAxis.y,yAxis.z,0,
		zAxis.x,zAxis.y,zAxis.z,0,
		0,0,0,1,
	};
	result = Matrix4x4::Transpose(result);
	result.m[3][0] = Vector3::Dot(xAxis, negpos);
	result.m[3][1] = Vector3::Dot(yAxis, negpos);
	result.m[3][2] = Vector3::Dot(zAxis, negpos);
	result.m[3][3] = 1;
	return result;
}

GE::Math::Matrix4x4 GE::Math::Matrix4x4::GetPerspectiveMatrix(float fov, float aspect, float nearClip, float farClip)
{
	Matrix4x4 result;
	float tanTheta = tanf(fov / 2);
	float x, y, width, height;
	x = 1.0f / tanTheta;
	y = 1.0f / tanTheta * aspect;
	width = 1 / (farClip - nearClip) * farClip;
	height = -nearClip / (farClip - nearClip) * farClip;
	result =
	{
		x,0,0,0,
		0,y,0,0,
		0,0,width,1,
		0,0,height,0
	};
	return result;
}

GE::Math::Matrix4x4 GE::Math::Matrix4x4::GetOrthographMatrix(const Vector2& size)
{
	Matrix4x4 result;
	result =
	{
		2.0f/size.x,0,0,0,
		0,-2.0f/size.y,0,0,
		0,0,1,0,
		-1,1,0,1
	};
	return result;
}

GE::Math::Matrix4x4 GE::Math::Matrix4x4::GetOrthographMatrix(const Vector2& size, float nearClip, float farClip)
{
	Matrix4x4 result;
	result =
	{
		2.0f / size.x,0,0,0,
		0,-2.0f / size.y,0,0,
		0,0,1 / (farClip - nearClip),0,
		0,nearClip / (nearClip - farClip),0,1
	};
	return result;
}

GE::Math::Matrix4x4 & GE::Math::operator*=(Matrix4x4 & m1, const Matrix4x4 & m2)
{
	Matrix4x4 result;

	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			for (int k = 0; k < 4; ++k)
			{
				result.m[i][j] += m1.m[i][k] * m2.m[k][j];
			}
		}
	}

	m1 = result;
	return m1;
}

GE::Math::Matrix4x4 GE::Math::operator*(const Matrix4x4 & m1, const Matrix4x4 & m2)
{
	Matrix4x4 result = m1;
	return result *= m2;
}

Vector3 GE::Math::operator*(const Vector3 & v, const Matrix4x4 & m)
{
	return Matrix4x4::Transform(v, m);
}
