#pragma once
#include"Plane.h"

class HUD : public Plane
{
public:
	HUD(Graphics& gfx, float xPos, float yPos, float zPos, std::string surfacePath);
	~HUD() = default;
	void TransformHUD(float _x, float _y, float _z, float _camPitch, float _camYaw, float _camRoll);
	void RotateHUD(float _pitch, float _yaw, float _roll);
	void SetHUDPosition(float _x, float _y, float _z);
	void SetHUDRotation(float _pitch, float _yaw, float _roll);
	float GetXHUDPos();
	float GetYHUDPos();
	float GetZHUDPos();
	float GetPitchHUDRot();
	float GetYawHUDRot();
	float GetRollHUDRot();
private:
	float xHUDPos = 0.0f;
	float yHUDPos = 0.0f;
	float zHUDPos = 0.0f;
	float pitchHUD = 0.0f;
	float yawHUD = 0.0f;
	float rollHUD = 0.0f;
};

