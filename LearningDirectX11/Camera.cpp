#include "Camera.h"
#include <algorithm>

namespace dx = DirectX;

DirectX::XMMATRIX Camera::GetMatrix()
{
	using namespace dx;

	const dx::XMVECTOR worldForward = dx::XMVectorSet(0, 0, 1, 0);

	const auto lookVector = dx::XMVector3Transform(
		worldForward,
		dx::XMMatrixRotationRollPitchYaw(pitch, yaw, roll)
	);

	const auto camPos = dx::XMVectorSet(xPos, yPos, zPos, 0);
	const auto camTarget = camPos + lookVector;

	return dx::XMMatrixLookAtLH(camPos, camTarget, dx::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));
}

DirectX::XMVECTOR Camera::GetForward()
{
	const dx::XMVECTOR worldForward = dx::XMVectorSet(0, 0, 1, 0);

	const auto directionVector = dx::XMVector3Transform(
		worldForward,
		dx::XMMatrixRotationRollPitchYaw(pitch, yaw, roll)
	);

	return directionVector;
}

DirectX::XMVECTOR Camera::GetRight()
{
	const dx::XMVECTOR worldRight = dx::XMVectorSet(1, 0, 0, 0);

	const auto directionVector = dx::XMVector3Transform(
		worldRight,
		dx::XMMatrixRotationRollPitchYaw(pitch, yaw, roll)
	);

	return directionVector;
}

DirectX::XMVECTOR Camera::GetUp()
{
	const dx::XMVECTOR worldUp = dx::XMVectorSet(0, 1, 0, 0);

	const auto directionVector = dx::XMVector3Transform(
		worldUp,
		dx::XMMatrixRotationRollPitchYaw(pitch, yaw, roll)
	);

	return directionVector;
}

float Camera::GetXPos()
{
	return xPos;
}

float Camera::GetYPos()
{
	return yPos;
}

float Camera::GetZPos()
{
	return zPos;
}

float Camera::GetPitch()
{
	return pitch;
}

float Camera::GetYaw()
{
	return yaw;
}

float Camera::GetRoll()
{
	return roll;
}

void Camera::SetPosition(float _x, float _y, float _z)
{
	xPos = _x;
	yPos = _y;
	zPos = _z;
}
// Sets in radians
void Camera::SetRotation(float _pitch, float _yaw, float _roll)
{
	pitch = _pitch;
	yaw = _yaw;
	roll = _roll;
}

void Camera::Translate(float _x, float _y, float _z)
{
	dx::XMFLOAT3 translation = dx::XMFLOAT3(_x, _y, _z);

	dx::XMStoreFloat3(&translation, dx::XMVector3Transform(
		dx::XMLoadFloat3(&translation),
		dx::XMMatrixRotationRollPitchYaw(pitch, yaw, roll)
	));

	xPos += translation.x;
	yPos += translation.y;
	zPos += translation.z;
}

void Camera::Rotate(float _pitch, float _yaw, float _roll)
{
	pitch = std::clamp(pitch + _pitch, (float)(0.995f * -3.14f / 2.0f), (float)(0.995f * 3.14 / 2.0f));
	yaw += _yaw;
	roll += _roll;
}