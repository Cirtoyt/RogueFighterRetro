#pragma once
#include "Box.h"

class Bullet : public Box
{
public:
	Bullet(Graphics& gfx, float xPos, float yPos, float zPos, std::string surfacePath);
	~Bullet() = default;
	void UpdatePosition(float dt);
	void SetForwardDirection(float _x, float _y, float _z);
	bool IsLifeUp(float dt);
private:
	float forwardX = 0.0f;
	float forwardY = 0.0f;
	float forwardZ = 0.0f;
	float bulletSpeed = 0.035f;
	float lifeTimer = 0.0f;
	float lifeSpan = 200.0f;
};

