#include "Bullet.h"


Bullet::Bullet(Graphics& gfx, float xPos, float yPos, float zPos, std::string surfacePath)
	:
	Box(gfx, xPos, yPos, zPos, surfacePath) { }

namespace dx = DirectX;

void Bullet::UpdatePosition(float dt)
{
	// Move bullet forward by speed along forward direction
	xPos += forwardX * bulletSpeed * dt;
	yPos += forwardY * bulletSpeed * dt;
	zPos += forwardZ * bulletSpeed * dt;
}

void Bullet::SetForwardDirection(float _x, float _y, float _z)
{
	forwardX = _x;
	forwardY = _y;
	forwardZ = _z;
}

bool Bullet::IsLifeUp(float dt)
{
	lifeTimer += dt;
	if (lifeTimer >= lifeSpan)
	{
		return true;
	}
	return false;
}
