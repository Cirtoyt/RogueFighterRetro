#include "Enemy.h"
#include <algorithm>

Enemy::Enemy(Graphics& gfx, float xPos, float yPos, float zPos, std::string surfacePath)
	:
	Plane(gfx, xPos, yPos, zPos, surfacePath) { }

namespace dx = DirectX;

void Enemy::RotateHUD(float _pitch, float _yaw, float _roll)
{
	pitch = std::clamp(pitch + _pitch, (float)(0.995f * -3.14f / 2.0f), (float)(0.995f * 3.14 / 2.0f));
	yaw += _yaw;
	roll += _roll;
}

void Enemy::UpdateFireTimer(float dt)
{
	fireTimer += dt;
}

bool Enemy::WantsToShoot()
{
	if (fireTimer >= fireRate)
	{
		fireTimer = 0.0f;
		return true;
	}
	return false;
}

DirectX::XMVECTOR Enemy::GetForward()
{
	const dx::XMVECTOR worldForward = dx::XMVectorSet(0, 0, -1, 0);

	const auto directionVector = dx::XMVector3Transform(
		worldForward,
		dx::XMMatrixRotationRollPitchYaw(pitch, yaw, roll)
	);

	return directionVector;
}
