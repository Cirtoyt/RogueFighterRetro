#include "HUD.h"
#include <algorithm>

HUD::HUD(Graphics& gfx, float xPos, float yPos, float zPos, std::string surfacePath)
	:
	Plane(gfx, xPos, yPos, zPos, surfacePath) { }

namespace dx = DirectX;

void HUD::TransformHUD(float _x, float _y, float _z, float _camPitch, float _camYaw, float _camRoll)
{
	dx::XMFLOAT3 translation = dx::XMFLOAT3(_x, _y, _z);

	dx::XMStoreFloat3(&translation, dx::XMVector3Transform(
		dx::XMLoadFloat3(&translation),
		dx::XMMatrixRotationRollPitchYaw(_camPitch, _camYaw, _camRoll)
	));

	xPos += translation.x;
	yPos += translation.y;
	zPos += translation.z;
}

void HUD::RotateHUD(float _pitch, float _yaw, float _roll)
{
	pitch = std::clamp(pitch + _pitch, (float)(0.995f * -3.14f / 2.0f), (float)(0.995f * 3.14 / 2.0f));
	yaw += _yaw;
	roll += _roll;
}

void HUD::SetHUDPosition(float _x, float _y, float _z)
{
	xHUDPos = _x;
	yHUDPos = _y;
	zHUDPos = _z;
}

void HUD::SetHUDRotation(float _pitch, float _yaw, float _roll)
{
	pitchHUD = pitch = _pitch;
	yawHUD = yaw = _yaw;
	rollHUD = roll = _roll;
}

float HUD::GetXHUDPos()
{
	return xHUDPos;
}

float HUD::GetYHUDPos()
{
	return yHUDPos;
}

float HUD::GetZHUDPos()
{
	return zHUDPos;
}

float HUD::GetPitchHUDRot()
{
	return pitchHUD;
}

float HUD::GetYawHUDRot()
{
	return yawHUD;
}

float HUD::GetRollHUDRot()
{
	return rollHUD;
}
