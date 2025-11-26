#include "../include/Transform.h"

GE::Math::Matrix4x4 GE::Transform::GetMatrix()
{
	//Math::Vector3 axis = Math::Vector3(1, 0, 0);
	//Math::Axis axis;

	//Math::Quaternion xQuat = Math::Quaternion(Math::Vector3(1,0,0), Math::ConvertToRadian(rotation.x));
	//Math::Quaternion yQuat = Math::Quaternion(Math::Vector3(0,1,0), Math::ConvertToRadian(rotation.y));
	//Math::Quaternion zQuat = Math::Quaternion(Math::Vector3(0,0,1), Math::ConvertToRadian(rotation.z));
	//Math::Quaternion resultQuat = zQuat * xQuat * yQuat;
	//Math::Quaternion quat = Math::Quaternion(rotation, Math::ConvertToRadian(rotation.Length()));
	//quat = Math::Quaternion::Normalize(quat);

	return Math::Matrix4x4::Scale(scale) * rotation.Rotation() * Math::Matrix4x4::Translate(position);
}

GE::Math::Vector3 GE::Transform::GetEulerAngle()
{
	return Math::Vector3();
}

GE::Math::Vector3 GE::Transform::GetForward()
{
	GE::Math::Axis axis = GetMatrix().GetAxis();
	return axis.z;
}

GE::Math::Vector3 GE::Transform::GetRight()
{
	GE::Math::Axis axis = GetMatrix().GetAxis();
	return axis.x;
}

GE::Math::Vector3 GE::Transform::GetUp()
{
	GE::Math::Axis axis = GetMatrix().GetAxis();
	return axis.y;
}
