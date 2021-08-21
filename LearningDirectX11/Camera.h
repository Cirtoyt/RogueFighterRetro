#pragma once
#include "Graphics.h"

class Camera
{
public:
	DirectX::XMMATRIX GetMatrix();
	DirectX::XMVECTOR GetForward();
	DirectX::XMVECTOR GetRight();
	DirectX::XMVECTOR GetUp();
	float GetXPos();
	float GetYPos();
	float GetZPos();
	float GetPitch();
	float GetYaw();
	float GetRoll();
	void SetPosition(float _x, float _y, float _z);
	// Sets in radians
	void SetRotation(float _pitch, float _yaw, float _roll);
	void Translate(float _x, float _y, float _z);
	void Rotate(float _pitch, float _yaw, float _roll);
private:
	float xPos = 0.0f;
	float yPos = 0.0f;
	float zPos = 0.0f;
	float pitch = 0.0f;
	float yaw = 0.0f;
	float roll = 0.0f;
};